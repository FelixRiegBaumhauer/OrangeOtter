#ifndef MESSAGE_ENTRY_H
#define MESSAGE_ENTRY_H

#include <string>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "message.h"

class MessageEntry{

	/* MessageEntries are used to keep track of the messages in the message map, to be used for filtering for invocation smenatics */

public:
  uint port;
  in_addr_t ip_addr;
  uint num;
  Message message;

  MessageEntry(uint port, in_addr_t ip_addr, uint num, Message message);
  void print();
  int compareTo(MessageEntry me);
  int compareTo(uint port, in_addr_t ip_addr, uint num);
  int compareHost(MessageEntry me);

};

#endif