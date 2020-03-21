#ifndef MESSAGE_ENTRY_H
#define MESSAGE_ENTRY_H

#include <string>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class MessageEntry{
public:
  uint port;
  in_addr_t ip_addr;
  uint num;

  MessageEntry(uint port, in_addr_t ip_addr, uint num);
  void print();
  int compareTo(MessageEntry me);

};

#endif