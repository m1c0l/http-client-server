#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <sys/stat.h>

#include "HttpMessage.h"
#include "HttpResponse.h"
#include "HttpRequest.h"

const int  BUFFER_SIZE = 200;

using namespace std;

void thread_func(sockaddr_in clientAddr, string filedir, int clientSockfd) {

	//        socklen_t clientAddrSize = sizeof(clientAddr);
	char ipstr[INET_ADDRSTRLEN] = {'\0'};
	inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
	std::cout << "Accept a connection from: " << ipstr << ":" <<
		ntohs(clientAddr.sin_port) << std::endl;

	char buf[BUFFER_SIZE] = {0};
	string httpTemp = "";
	int startInd =0;
	HttpRequest message;
	int status = 200;

	// read/write data from/into the connection
	while (true)
	{
		memset(buf, '\0', sizeof(buf));

		if (recv(clientSockfd, buf, BUFFER_SIZE, 0) == -1) {
			perror("recv");
			return;
		}

		httpTemp += buf;

		size_t x = httpTemp.find(CRLF, startInd);
		if(x == string::npos) {
			startInd += BUFFER_SIZE;
		}
		else
		{
			httpTemp.resize(x+5);
			int decodeStatus = message.decode(httpTemp);
			if(decodeStatus) {
				status = decodeStatus;
			}
			break;
		}
	}

	//*

	string url = message.getUrl();//need to decide when/how to process absolute url vs ppath

	fstream wantedFile;
	string filepath = filedir + url;
	// find the http requested file in the file directory
	wantedFile.open(filepath);
	if(!wantedFile)
	{
		perror("fstream::open");
		status = 404;
	}

	// find body length
	off_t bodyLength = 0;
	struct stat st;
	if (stat(filepath.c_str(), &st) == 0) {
		bodyLength = st.st_size;
	}
	else {
		perror("stat");
		status = 404;
	}

	string fileBody, holder;
	while(getline(wantedFile,holder))
		fileBody+=(holder+ '\n');

	//cout << fileBody << endl;

	HttpResponse response;
	response.setVersion("HTTP/1.0");
	response.setStatus(to_string(status)); // or other error cases
	response.setDescription("OK"); //Other error cases
	if (status == 200) {
		response.setHeader("Content-Length", to_string(bodyLength));
	}

	string responseMessage = response.encode();//Takes care of first line
	//cout << responseMessage << endl;
	//*/

	send(clientSockfd, responseMessage.c_str(), responseMessage.size(), 0);

	close(clientSockfd);
}

int main(int argc, char **argv) {
	if (argc > 4) {
		cerr << "usage: " << argv[0] << " [hostname] [port] [file-dir]" << '\n';
		return 1;
	}

	string hostname, port, filedir;

	// check argc and set default parameters if needed
	hostname = argc < 2 ? "localhost" : argv[1];
	port = argc < 3 ? "4000" : argv[2];
	filedir = argc < 4 ? "." : argv[3];

	// create a socket using TCP IP
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// allow others to reuse the address
	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		return 1;
	}

	// bind address to socket
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(stoi(port));     // short, network byte order
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("bind");
		return 2;
	}

	// set socket to listen status
	if (listen(sockfd, 1) == -1) {
		perror("listen");
		return 3;
	}

	// accept a new connection
	struct sockaddr_in clientAddr;

	int clientSockfd;

	socklen_t clientAddrSize = sizeof(clientAddr);
	//Start Multithreading Magic
	while( (clientSockfd= accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize))){

		if (clientSockfd == -1) {
			perror("accept");
			return 4;
		}
		thread{thread_func, clientAddr, filedir, clientSockfd}.detach();

	}



	return 0;
}
