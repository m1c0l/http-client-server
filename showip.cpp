#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo* res;

  if (argc != 2) {
    std::cerr << "usage: showip hostname" << std::endl;
    return 1;
  }

  // prepare hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP

  // get address
  int status = 0;
  if ((status = getaddrinfo(argv[1], "80", &hints, &res)) != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
    return 2;
  }

  std::cout << "IP addresses for " << argv[1] << ": " << std::endl;

  for(struct addrinfo* p = res; p != 0; p = p->ai_next) {
    // convert address to IPv4 address
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;

    // convert the IP to a string and print it:
    char ipstr[INET_ADDRSTRLEN] = {'\0'};
    inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
    std::cout << "  " << ipstr << std::endl;
    // std::cout << "  " << ipstr << ":" << ntohs(ipv4->sin_port) << std::endl;
  }

  freeaddrinfo(res); // free the linked list

  return 0;
}
