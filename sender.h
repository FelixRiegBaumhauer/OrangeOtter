#ifndef SENDER_H
#define SENDER_H


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

#define INT_SIZE 4
#define TIMEOUT_DURATION 10
#define NUM_TIMEOUTS 3

//this is for the responses
typedef enum success_type {
	Good = 0,
	Failure = 1,
	OtherFailure = 2
} SuccessType;

class Sender{
public:

	Marshal marshal;

	//returns the number of packets waiting and waits up to seconds seconds of time
	int input_timeout (int filedes, unsigned int seconds);

	void populateLocalSockAddr(struct sockaddr_in *sa);
	void populateRemoteSockAddr(struct sockaddr_in *sa, char * hostname, int port);


	Message sendMessage(Message call, int sockfd, struct sockaddr_in *sa);
	Message recvMessage(int sockfd,  struct sockaddr_in * sa);
	int sendResponse(Message m, int sockfd,  struct sockaddr_in * sa);

/*
	int sendMessage(Message m, int sockfd, struct sockaddr_in *sa);

	//for the recving message we assume that it is zeroed out 
	int recvMessage(Message * m, int sockfd, struct sockaddr_in *sa);
*/



	//reads in chunks for big messages returns number of bytes read
	int recvWholeStream(int sockfd, char ** buf, struct sockaddr_in * sa);




};




#endif