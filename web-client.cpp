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
#include <sstream>

using namespace std;

int main(int argc, char **argv) {
	if (argc != 2) {
		cerr << "Usage: ./webclient [URL]\n";	
		return 1;
	}
	string urlStr = argv[1];
	
	struct addrinfo hints;
	struct addrinfo* res;

	// prepare hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP

	// get address
	int status = 0;
	if ((status = getaddrinfo(argv[1], "80", &hints, &res)) != 0) {
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
	serverAddr.sin_port = htons(40000);     // short, network byte order
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
	char buf[20] = {0};
	stringstream ss;

	while (!isEnd) {
		memset(buf, '\0', sizeof(buf));

		cout << "send: ";
		cin >> input;
		if (send(sockfd, input.c_str(), input.size(), 0) == -1) {
			perror("send");
			return 4;
		}


		if (recv(sockfd, buf, 20, 0) == -1) {
			perror("recv");
			return 5;
		}
		ss << buf << '\n';
		cout << "echo: ";
		cout << buf << '\n';

		if (ss.str() == "close\n")
			isEnd = true;

		ss.str("");
	}

	close(sockfd);

	return 0;
}
