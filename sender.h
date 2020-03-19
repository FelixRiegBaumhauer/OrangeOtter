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


#define MAXLINE 1024 



class Sender{
public:

	Marshal marshal;

	void populateLocalSockAddr(struct sockaddr_in *sa);
	void populateRemoteSockAddr(struct sockaddr_in *sa, char * hostname, int port);

	int sendMessage(Message m, int sockfd, struct sockaddr_in *sa);

	//for the recving message we assume that it is zeroed out 
	int recvMessage(Message * m, int sockfd, struct sockaddr_in *sa);


};




#endif