#ifndef MESSAGE_ENTRY_H
#define MESSAGE_ENTRY_H

#include <string>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class MessageEntry{

	/* MessageEntries are used to keep track of the messages in the message map, to be used for filtering for invocation smenatics */

public:
  uint port;
  in_addr_t ip_addr;
  uint num;

  MessageEntry(uint port, in_addr_t ip_addr, uint num);
  void print();
  int compareTo(MessageEntry me);
  int compareHost(MessageEntry me);

};

#endif