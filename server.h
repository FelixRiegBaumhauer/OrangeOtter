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


class Server{
public:
	//need to add the ds for filtering
	uint num;

	void updateNum();
	uint getNum();

	Server();
	int server_loop(int port);

};


#endif