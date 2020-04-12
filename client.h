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


#define DEFAULT_T 5
#define DEFAULT_CLIENT_MODE NormalClient
#define DEFAULT_PROB 0
#define DEFAULT_SEMANTIC AtMostOnce

typedef enum invocation_semantic {
	AtMostOnce = 0,
	AtLeastOnce = 1
} InvocationSemantic;

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
	int prevPacketNumber;
	InvocationSemantic semantic;

	Client();
	Client(ClientMode mode, float dropProb, uint t, InvocationSemantic semantic);

	/*
	The client loop serves as the main infite loop of the program, the user inputs instructions and recieves output
	*/
	int client_loop(int server_port, int client_port, in_addr_t server_ip, in_addr_t client_ip);

	/* Processes reponse, ie prints the message or other actions */
	void processResponse(Message m, int sockfd, struct sockaddr_in * sa);

	/* Handles the monitor instruction, which is to wait during the interval and simpily print all monitor updates*/
	Message handleMonitor(Message m, int sockfd, struct sockaddr_in * sa);

	/* Use to wait for the monitor handler */
	int input_timeout (int filedes, unsigned int seconds);

	/* cehcks the map, ie the prevPacketNum, to see that all is good, returns 0 if good, 1 if duplicate */
	int checkMap(Message m);
};


#endif