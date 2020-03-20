#ifndef CLIENT_H
#define CLIENT_H


#include <string>
#include <iostream>
#include <vector>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include "marshal.h"
#include "sender.h"


class Client{
public:
	uint num;

	void updateNum();
	uint getNum();

	Client();
	int client_loop(int server_port, int client_port);




};



#endif