#ifndef CLIENT_H
#define CLIENT_H


#include <string>
#include <iostream>
#include <vector>
#include <ctime>

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
#include "cache.h"
#include "filesystem.h"


#define DEFAULT_T 60
#define DEFAULT_CLIENT_MODE NormalClient
#define DEFAULT_PROB 0


typedef enum client_mode {
	NormalClient = 0,
	DroppingClient = 1
} ClientMode;


class Client{
public:
    Sender sender;
    Cache cache;
    FileSystem fs;

	ClientMode mode;
	float dropProb;
	uint t;

	Client();
	Client(ClientMode mode, float dropProb, uint t);
	//int client_loop(int server_port, int client_port);
	int client_loop(int server_port, int client_port, in_addr_t server_ip, in_addr_t client_ip);

	void processResponse(Message m, int sockfd, struct sockaddr_in * sa);
	Message handleMonitor(Message m, int sockfd, struct sockaddr_in * sa);
	int input_timeout (int filedes, unsigned int seconds);



};



#endif