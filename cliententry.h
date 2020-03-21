#ifndef CLIENT_ENTRY_H
#define CLIENT_ENTRY_H

#include <string>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class ClientEntry{
public:
  uint port;
  in_addr_t ip_addr;
  uint clientNum;

  ClientEntry(uint port, in_addr_t ip_addr, uint clientNum);
  void print();
  int compareTo(ClientEntry ce);
  int find(struct sockaddr_in cliaddr);

};

#endif