#ifndef CLIENT_MAP_H
#define CLIENT_MAP_H

#include <string>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "cliententry.h"
#include "exception.h"

class ClientMap{

	/* The client map is set of all client entreries so we know which users are currently using the service */

public:
	uint curNum;
	std::vector<ClientEntry> map;

	ClientMap();

	/* Finds the client num based on the cliadr ie ip and port, if no client entry found add one  */
	uint findClientNum(struct sockaddr_in cliaddr);

	/*  Simpily find an existing ClientEntry*/
	ClientEntry getClientEntry(uint ClientNum);
};

#endif