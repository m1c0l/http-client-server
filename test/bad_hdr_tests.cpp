#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;	
int main(int argc, char **argv) {
	// Make a small README so server doesn't crash!
	vector<string> tests = {
		"GET /README.md HTTP/1.0\r\n\r\n", // should work, receive status 200
		"POST /README.md HTTP/1.0\r\n\r\n", // should receive error 501
		"GET /README.md HTTP/1.1\r\n\r\n", // should receive error 505
		"GET /README.md HTTP/1.2\r\n\r\n", // should receive error 400
		"GET\r\n\r\n", // should receive error 400
		"\r\n\r\n", // error 501,
		"GET /README.md HTTP/1.0\r\n:\r\n\r\n", // 400
		"GET /README.md HTTP/1.0\r\n : \r\n\r\n", // 200
		"GET /README.md HTTP/1.0\r\nblah: abc\r\n\r\n", // 200
		"GET /README.md HTTP/1.0\r\nblah: abc\r\nb:a\r\n\r\n", // 200
		"GET /README.md HTTP/1.0\r\nblah: abc\r\n1:2\r\n3:4\r\n\r\n", //200 since we're nice
		"GET /README.md HTTP/1.0\r\n blah \r\n\r\n", // 400
		"GET /README.md HTTP/1.0\r\na:\r\n\r\n", // 400
		"GET /README.md HTTP/1.0\r\n:b\r\n\r\n" // 400
	};

	for (string input : tests) {

		// create a socket using TCP IP
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);

		struct sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(4000);     // short, network byte order
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//"169.232.126.218");
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

		char ipstr[INET_ADDRSTRLEN] = {'\0'};
		inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
		std::cout << "Set up a connection from: " << ipstr << ":" <<
			ntohs(clientAddr.sin_port) << std::endl;


		// send/receive data to/from connection
		bool isEnd = false;
		//std::string input;
		char buf[50] = {0};
		std::stringstream ss;

		while (!isEnd) {
			memset(buf, '\0', sizeof(buf));

			std::cout << "sent:\n"<< input;



			if (send(sockfd, input.c_str(), input.size(), 0) == -1) {
				perror("send");
				return 4;
			}

			if (recv(sockfd, buf, 50, 0) == -1) {
				perror("recv");
				return 5;
			}
			ss << buf << std::endl;
			std::cout << "client echo: ";
			std::cout << buf << std::endl;
			break;
			if (ss.str() == "close\n")
				break;

			ss.str("");
		}

		close(sockfd);
		sleep(2);
	}
	return 0;
}
