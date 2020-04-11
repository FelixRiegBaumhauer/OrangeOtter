
#include "messageentry.h"

MessageEntry::MessageEntry(uint port, in_addr_t ip_addr, uint num){
  this->port = port;
  this->ip_addr = ip_addr;
  this->num = num;
}

void MessageEntry::print(){
  std::cout << "Port: " << port << " IP Addr: " << ip_addr << " Num: " << num << std::endl;
}

//returns 1 if true, 0 if false
int MessageEntry::compareTo(MessageEntry me){
	if(this->port == me.port && this->ip_addr == me.ip_addr && this->num == me.num){
		return 1;
	}
	return 0;
}

//returns 1 if true, 0 if false
int MessageEntry::compareHost(MessageEntry me){
	if(this->port == me.port && this->ip_addr == me.ip_addr){
		return 1;
	}
	return 0;
}