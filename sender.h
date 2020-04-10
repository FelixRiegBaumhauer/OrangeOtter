#ifndef SENDER_H
#define SENDER_H


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
#include "exception.h"

#define INT_SIZE 4
#define TIMEOUT_DURATION 10
#define NUM_TIMEOUTS 3

//this is for the responses
typedef enum success_type {
	Good = 0,
	Failure = 1
} SuccessType;

typedef enum sender_mode {
	NormalSender = 0,
	DroppingSender = 1
} SenderMode;

class Sender{

	/* The sender is used to send all messages, it uses the marshal and then sends out via UDP */

public:

	float dropProb;
	SenderMode senderMode;
	Marshal marshal;
	uint messageNum;

	
	Sender();
	Sender(float dropProb);
	
	/* Accounting to get the numbers right */
	void updateNum();
	uint getNum();
	uint getUpdateNum();

	//return 1 yes drop it, 0 no do not drop it
	/* Used for simulations, takes the probabillity of the drop to 'drop' a package */
	uint toDrop();

	/* returns the number of packets waiting and waits up to seconds seconds of time */
	int input_timeout (int filedes, unsigned int seconds);

	/* Set up sock addr */
	void populateLocalSockAddr(struct sockaddr_in *sa);
	void populateRemoteSockAddr(struct sockaddr_in *sa, char * hostname, int port);
	void populateRemoteSockAddr(struct sockaddr_in *sa, in_addr_t host_ip, int port);

	/* Send and recieve messages */
	Message sendMessage(Message call, int sockfd, struct sockaddr_in *sa);
	Message recvMessage(int sockfd,  struct sockaddr_in * sa);
	int sendResponse(Message m, int sockfd,  struct sockaddr_in * sa);

	//reads in chunks for big messages returns number of bytes read
	int recvWholeStream(int sockfd, char ** buf, struct sockaddr_in * sa);
};

#endif