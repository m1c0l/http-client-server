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

#include "HttpMessage.h"
#include "HttpResponse.h"
#include "HttpRequest.h"

const int  BUFFER_SIZE = 200;

using namespace std;

int main(int argc, char **argv) {
	if (argc > 4) {
		cerr << "usage: web-server [hostname] [port] [file-dir]" << '\n';
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
	socklen_t clientAddrSize = sizeof(clientAddr);
	int clientSockfd;
	
	//Start Multithreading Magic
	while( clientSockfd= accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize)){

	if (clientSockfd == -1) {
		perror("accept");
		return 4;
	}

	}

	
	char ipstr[INET_ADDRSTRLEN] = {'\0'};
	inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
	std::cout << "Accept a connection from: " << ipstr << ":" <<
		ntohs(clientAddr.sin_port) << std::endl;

	// read/write data from/into the connection
	bool isEnd = false;
	char buf[BUFFER_SIZE] = {0};
	std::stringstream ss;
	stringstream temp;

	string httpTemp = "";
	
	int startInd =0;
	//int endInd = BUFFER_SIZE-1;
	HttpRequest message;
	string errorStatus = "200";
	
	while (!isEnd) {
		memset(buf, '\0', sizeof(buf));

		if (recv(clientSockfd, buf, BUFFER_SIZE, 0) == -1) {
			perror("recv");
			return 5;
		}
		
		
		if (temp.str() == "close\n")
		  break;
		//ss << buf << std::endl;
		//std::cout << buf << std::endl;
				
		int x;

		temp << buf << endl;
		httpTemp += buf;
		//cout << httpTemp << endl;
		x = httpTemp.find(CRLF, startInd);
		if(x == -1)
		  {startInd += BUFFER_SIZE;}
		else
		  {
		    httpTemp.resize(x+5); //httpTemp is string of Http message, maybe change to stringstream later? 
		    if (message.decode(httpTemp) == 400) {
		    	errorStatus = "400";
		    }
		    break;

		    /*  if (send(clientSockfd, httpTemp,httpTemp.size(), 0) == -1) {
			perror("send");
			return 6;
			}*/
		  }

		/*
		if (temp.str() == "close\n"){
		 std::cout << "BREAK" << std::endl;
		  isEnd = true;
		  break;
		}
		*/
		temp.str("");
	}
	
	//cout << httpTemp << endl;
	//*
	
	string url = message.getUrl();//need to decide when/how to process absolute url vs ppath
	
	fstream wantedFile;
	wantedFile.open(filedir+url); //find the http requested file in the file directory
	if(!wantedFile)
	  {
	    //cerr << "Open Failure\n";
	    errorStatus = "404";
	    
	  }
	
	string fileBody, holder;
	while(getline(wantedFile,holder))
	  fileBody+=(holder+ '\n'); 
	
	//cout << fileBody << endl;
	
	HttpResponse response;
	response.setBody(fileBody);
	response.setVersion("HTTP/1.0");
	response.setStatus(errorStatus); // or other error cases
	response.setDescription("OK"); //Other error cases
     
     
	
	response.setHeader("Content-Length", to_string(fileBody.size()));
	
	string responseMessage = response.encode();//Takes care of first line
	//cout << responseMessage << endl;
	//*/
	
	send(clientSockfd, responseMessage.c_str(), responseMessage.size(), 0);

	close(clientSockfd);

	return 0;
}
