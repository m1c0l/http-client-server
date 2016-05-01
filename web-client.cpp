#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <libgen.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <future>

#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;

const int BUFFER_SIZE = 200;

struct Url {
	string host;
	string port;
	string path;
};

Url parseUrl(string urlStr) {
	// remove http:// at start of URL
	if (urlStr.substr(0, 7) == "http://") {
		urlStr = urlStr.substr(7);
	}

	size_t colonPos = urlStr.find_first_of(":");
	size_t slashPos = urlStr.find_first_of("/");

	string hostname = urlStr;
	if (colonPos != string::npos) {
		hostname = urlStr.substr(0, colonPos);
	}
	else if (slashPos != string::npos) {
		hostname = urlStr.substr(0, slashPos);
	}

	string port = "80";
	if (colonPos != string::npos && slashPos != string::npos) {
		port = urlStr.substr(colonPos + 1, slashPos - colonPos - 1);
	}
	else if (colonPos != string::npos) {
		port = urlStr.substr(colonPos + 1);
	}

	string filename = "/";
	if (slashPos != string::npos) {
		filename = urlStr.substr(slashPos);
	}

	Url url;
	url.host = hostname;
	url.port = port;
	url.path = filename;
	return url;
}

struct sockaddr_in getServerAddr(string hostname, string port) {
	struct addrinfo hints;
	struct addrinfo* res;

	// prepare hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP

	// get address
	int status = 0;
	if ((status = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &res)) != 0) {
		cerr << "getaddrinfo: " << gai_strerror(status) << '\n';
		exit(2);
	}

	struct addrinfo* p = res;
	// convert address to IPv4 address
	struct sockaddr_in* ipv4;
	char ipstr[INET_ADDRSTRLEN] = {'\0'};
	if (p != 0) {
		ipv4 = (struct sockaddr_in*)p->ai_addr;

		// convert the IP to a string
		inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
	}
	else {
		cerr << "IP address not found for " << hostname << endl;
		exit(3);
	}

	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(stoi(port));     // short, network byte order
	serverAddr.sin_addr.s_addr = inet_addr(ipstr);
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
	return serverAddr;
}

int getResponse(int sockfd, sockaddr *serverAddr, HttpRequest* req,
		string filename) {
	// connect to the server
	if (connect(sockfd, serverAddr, sizeof(*serverAddr)) == -1) {
		perror("connect");
		return 4;
	}

	string msg = req->encode();
	if (send(sockfd, msg.c_str(), msg.size(), 0) == -1) {
		perror("send");
		return 5;
	}

	char buf[BUFFER_SIZE + 1] = {0};
	string resBuf;
	HttpResponse* res = new HttpResponse();

	string eoh = "\r\n\r\n"; // end of header
	int recv_status = 0;

	while (true) {
		memset(buf, '\0', sizeof(buf));

		recv_status = recv(sockfd, buf, BUFFER_SIZE, 0);
		if (recv_status == -1) {
			perror("recv");
			return 6;
		}
		resBuf += buf;

		// when the entire header has been read, decode it
		size_t pos = resBuf.find(eoh);
		if (pos != string::npos) {
			size_t end = pos + eoh.size();

			// decode the header
			res->decode(resBuf.substr(0, end));

			// discard the header but keep the rest of the buffer
			resBuf = resBuf.substr(end, resBuf.size());
			break;
		}

		// if end of header is not detected but no incoming data
		if (recv_status == 0) {
			cerr << "Incomplete HTTP response" << '\n';
			return 7;
		}
	}


	// 200 OK
	if (res->getStatus() == "200") {
		// get file name
		string filenameCopy = filename.c_str();
		string base = basename((char*)filenameCopy.c_str());
		if (filename[filename.size() - 1] == '/') {
			base = "index.html";
		}

		// save retrieved file
		ofstream of;
		of.open(base);

		of << resBuf; // leftover data from reading header

		while (true) {
			memset(buf, '\0', sizeof(buf));

			recv_status = recv(sockfd, buf, BUFFER_SIZE, 0);
			if (recv_status == -1) {
				perror("recv");
				of.close();
				return 8;
			}
			if (recv_status == 0) {
				of.close();
				break;
			}

			of << buf;
		}
	}
	// not 200, just output the status and description
	else {
		cout << res->getStatus() + " " + res->getDescription() << '\n';
	}

	return 0;
}


int main(int argc, char **argv) {
	if (argc < 2) {
		cerr << "usage: " << argv[0] << " [URL] [URL] ..." << '\n';
		return 1;
	}

	int resp_status = 0;

	// send a request for each url
	for (int i = 1; i < argc; i++) {

		Url url = parseUrl(argv[i]);
		sockaddr_in serverAddr = getServerAddr(url.host, url.port);

		// set up HTTP request
		HttpRequest* req = new HttpRequest();
		req->setMethod("GET");
		req->setUrl(url.path);
		req->setVersion("HTTP/1.0");
		req->setHeader("Host", url.host);

		// create a socket using TCP IP
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);

		// send the request
		chrono::seconds timer(30);
		future<int> promise = async(launch::async, getResponse,
				sockfd, (sockaddr*)&serverAddr, req, url.path);

		// abort if request times out
		if (promise.wait_for(timer) == future_status::timeout) {
			close(sockfd);
			cerr << "Connection timed out." << '\n';
			return 2;
		}
		resp_status = promise.get();
		close(sockfd);
	}

	return resp_status;
}
