#include "cliententry.h"

ClientEntry::ClientEntry(uint port, in_addr_t ip_addr, uint clientNum){
  this->port = port;
  this->ip_addr = ip_addr;
  this->clientNum = clientNum;
}

void ClientEntry::print(){
  std::cout << "Port: " << port << " IP Addr: " << ip_addr << " Client Num: " << clientNum << std::endl;
}

//returns 1 if true, 0 if false
int ClientEntry::compareTo(ClientEntry ce){
	if(this->port == ce.port && this->ip_addr == ce.ip_addr && this->clientNum == ce.clientNum){
		return 1;
	}
	return 0;
}

int ClientEntry::find(struct sockaddr_in cliaddr){
	if(this->port == ((uint) cliaddr.sin_port) && this->ip_addr == cliaddr.sin_addr.s_addr){
		return 1;
	}
	return 0;
}