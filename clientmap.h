#ifndef CLIENT_MAP_H
#define CLIENT_MAP_H

#include <string>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "cliententry.h"

class ClientMap{
public:
	uint curNum;
	std::vector<ClientEntry> map;

	uint findClientNum(struct sockaddr_in cliaddr);
	ClientMap();
	ClientEntry getClientEntry(uint ClientNum);
};

#endif