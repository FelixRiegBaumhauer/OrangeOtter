#ifndef SERVER_H
#define SERVER_H

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <vector>

#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/time.h>


#include "marshal.h"
#include "sender.h"
#include "messageentry.h"
//#include "cliententry.h"
#include "clientmap.h"


class Server{
public:
	std::vector<MessageEntry> messageMap;
	
	ClientMap clientMap;
	//std::vector<ClientEntry> clientMap;
	//need to add the ds for filtering
	uint num;

	void updateNum();
	uint getNum();

	Server();
	int server_loop(int port);
	int checkMap(Message m, struct sockaddr_in cliaddr);
	Message execute(Message call);
};


#endif