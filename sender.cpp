
#include "sender.h"


  
int Sender::input_timeout (int filedes, unsigned int seconds){
    fd_set set;
    struct timeval timeout;
    int n;

    /* Initialize the file descriptor set. */
    FD_ZERO (&set);
    FD_SET (filedes, &set);

    /* Initialize the timeout data structure. */
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;

    /* select returns 0 if timeout, 1 if input available, -1 if error. */
    if((n = select(FD_SETSIZE, &set, NULL, NULL, &timeout)) < 0){
        printf("ERROR\n");
        return -1;
    }
    return n;
}


void Sender::populateLocalSockAddr(struct sockaddr_in *sa){
	memset(sa, 0, sizeof(*sa)); 
      
    sa->sin_family = AF_INET; 
    sa->sin_addr.s_addr = INADDR_ANY;  
    sa->sin_port = htons(0); 
}

void Sender::populateRemoteSockAddr(struct sockaddr_in *sa, char * hostname, int port){
	memset(sa, 0, sizeof(*sa)); 
      
    sa->sin_family = AF_INET; 
    sa->sin_addr.s_addr = inet_addr(hostname);  
    sa->sin_port = htons(port); 
}


int Sender::sendMessage(Message m, int sockfd, struct sockaddr_in *sa){
	unsigned char * byte_stream;
	uint stream_len, ack_len;
    int n, len, packets_waiting, i;

    char buffer [MAXLINE];
    Message ack;


    byte_stream = marshal.marshalMessage(m, &stream_len);

    //the idea of this do while loop is to check if we have an ack waiting, 
    i = 0;
    do{
	    sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) sa, sizeof(*sa)); 
	    packets_waiting = input_timeout(sockfd, TIMEOUT_DURATION);
	    //test if packets waiting is -1 then error out

	    i+=1;
    } while(packets_waiting == 0 && i < NUM_TIMEOUTS);
    if(i == NUM_TIMEOUTS){
    	printf("TIMEOUT ERROR\n");
    	return 1;
    }
    free(byte_stream);
    
/*
    //now send it out, 
    sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) sa, sizeof(*sa)); 
    free(byte_stream);
*/
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) sa, (socklen_t *) &len);
    ack = marshal.unmarshalMessage((unsigned char *)buffer, &ack_len);
    //here we would check the Ack
    if(ack.callType != Ack || ack.type != AckType){
    	printf("Ack error\n");
    	return 1;
    }

    return 0;
}

//we must assume that cliaddr has been wiped clean for us
int Sender::recvMessage(Message * m, int sockfd,  struct sockaddr_in * sa){
    //char buffer[MAXLINE]; 
    char * buf;
    unsigned char * byte_stream;
    int len, n, i;
    uint stream_len, call_len;
    Message ack;
    struct sockaddr_in cliaddr1;

    len = sizeof(*sa);

    n = recvWholeStream(sockfd, &buf, sa);


    *m = marshal.unmarshalMessage((unsigned char *)buf, &call_len);

    ack = Message(AckType, Ack, {}, {});
    byte_stream = marshal.marshalMessage(ack, &stream_len);


    sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) sa, len); 
    free(byte_stream);

    return 0;
}

//TO BE CONTINUED

int Sender::recvWholeStream(int sockfd, char ** buf, struct sockaddr_in * sa){
	int len, n, i;
	uint byte_len;
	
	unsigned char small_buf [INT_SIZE];
	//char * small_buf;
	char * temp_buf;
	len = sizeof(*sa);

	//small_buf = (char *) malloc(INIT_PACKET_SIZE);

	//n = recvfrom(sockfd, (char *)small_buf, INIT_PACKET_SIZE, MSG_WAITALL, ( struct sockaddr *) sa, (socklen_t *)&len); 

	n = recvfrom(sockfd, (char *)small_buf, INT_SIZE, MSG_PEEK | MSG_WAITALL, ( struct sockaddr *) sa, (socklen_t *)&len); 
	byte_len = marshal.getMarshalLength((unsigned char *)small_buf);

	temp_buf = (char *) malloc(byte_len);

	//now read the rest
	//n += recvfrom(sockfd, (char *)( (temp_buf)+n), byte_len-n, MSG_WAITALL, ( struct sockaddr *) sa, (socklen_t *)&len);
	n = recvfrom(sockfd, (char *)temp_buf, byte_len, MSG_WAITALL, ( struct sockaddr *) sa, (socklen_t *)&len);

	//read the first byte, get size and then read the rest
	//add a method to the marshal that only reads out the size and returns it

	*buf = temp_buf;
	return n;
}

