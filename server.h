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
#include "clientmap.h"
#include "filesystem.h"


#define DEFAULT_INVOCATION_SEMANTIC AtMostOnce
#define DEFAULT_SERVER_MODE NormalServer
#define DEFAULT_PROB 0

typedef enum invocation_semantic {
	AtMostOnce = 0,
	AtLeastOnce = 1
} InvocationSemantic;

typedef enum server_mode {
	NormalServer = 0,
	DroppingServer = 1
} ServerMode;



class Server{

	/* Server is the distrbted cental file server */

public:
	FileSystem fs;
	std::vector<MessageEntry> messageMap;
	
	InvocationSemantic semantic;
	ServerMode mode;
	float dropProb;

    Sender sender;
	ClientMap clientMap;

	Server();
	Server(InvocationSemantic semantic, ServerMode mode, float dropProb);

	/* The loop is where we wait for commands and act on them */
	int server_loop(int port, in_addr_t serverIp);

	/* Check the message map to see if we have a collision, for At-Most-Once semantics */
	int checkMap(Message m, struct sockaddr_in cliaddr, InvocationSemantic semantic);

	/* Execute the servers response */
	Message execute(int sockfd, Message call, uint clientNum);

	/* Send a monitor message to all the listed clients */
	void sendList(int sockfd, std::vector<uint> clientNums, std::string filepath);
};


#endif