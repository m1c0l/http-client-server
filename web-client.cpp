#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#include <iostream>

#include "HttpRequest.h"
#include "HttpResponse.h"

const int BUFFER_SIZE = 200;

using namespace std;

int main(int argc, char **argv) {
	if (argc != 2) {
		cerr << "Usage: ./webclient [URL]\n";	
		return 1;
	}

	// process URL
	string urlStr = argv[1];
	if (urlStr.substr(0, 7) == "http://") {
		// remove http:// at start of URL
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

	cout << "hostname: " << hostname << " port: " << port << " filename: " << filename << "\n";	
	
	struct addrinfo hints;
	struct addrinfo* res;

	// prepare hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP

	// get address
	int status = 0;
	if ((status = getaddrinfo(hostname.c_str(), "80", &hints, &res)) != 0) {
		cerr << "getaddrinfo: " << gai_strerror(status) << '\n'; 
		return 2;
	}

	cout << "IP addresses for " << argv[1] << ": " << '\n';
	struct addrinfo* p = res;
	// convert address to IPv4 address
	struct sockaddr_in* ipv4;
	char ipstr[INET_ADDRSTRLEN] = {'\0'};
	if (p != 0) {
		ipv4 = (struct sockaddr_in*)p->ai_addr;

		// convert the IP to a string and print it:
		inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
		cout << "  " << ipstr << "\n";
	}

	// create a socket using TCP IP
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(stoi(port));     // short, network byte order
	serverAddr.sin_addr.s_addr = inet_addr(ipstr);
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	// connect to the server
	if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		perror("connect");
		return 2;
	}

	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
		perror("getsockname");
		return 3;
	}

	// send/receive data to/from connection
	bool isEnd = false;
	string input;
	char buf[BUFFER_SIZE] = {0};

	HttpRequest req;
	req.setMethod("GET");
	req.setUrl(filename);
	req.setVersion("HTTP/1.0");
	req.setHeader("Host", hostname);
	string msg = req.encode();

	if (send(sockfd, msg.c_str(), msg.size(), 0) == -1) {
		perror("send");
		return 4;
	}

	string responseBuf;
	while (!isEnd) {
		memset(buf, '\0', sizeof(buf));
		
		int recv_status = 0;
		if ((recv_status = recv(sockfd, buf, BUFFER_SIZE, 0)) == -1) {
			perror("recv");
			return 5;
		}
		else if (!recv_status) {
			break;
		}
		responseBuf += buf;
	}
	close(sockfd);

	HttpResponse resp;
	resp.decode(responseBuf);
	cout << resp.getBody();


	return 0;
}
