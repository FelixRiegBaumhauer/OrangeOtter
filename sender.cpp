
#include "sender.h"



void Sender::updateNum(){
    messageNum++;
}
uint Sender::getNum(){
    return messageNum;
}

uint Sender::getUpdateNum(){
    uint tempNum = messageNum;
    messageNum++;
    return tempNum;
}


Sender::Sender(){
    messageNum = 500;
}

  
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

void Sender::populateRemoteSockAddr(struct sockaddr_in *sa, in_addr_t host_ip, int port){
    memset(sa, 0, sizeof(*sa)); 
      
    sa->sin_family = AF_INET; 
    sa->sin_addr.s_addr = host_ip; //is allready in network order
    sa->sin_port = htons(port); 
}


Message Sender::sendMessage(Message call, int sockfd, struct sockaddr_in *sa){
	unsigned char * byte_stream;
	uint stream_len, resp_len; //, ack_len;
    int n, len, packets_waiting, i;
    //char buffer [MAXLINE];
    Message resp;


    //first we update the message num
    call.setNum(getUpdateNum());


    //basic idea is that we first send a call, wait for response and then return that


    byte_stream = marshal.marshalMessage(call, &stream_len);

    //the idea of this do while loop is to check if we have an response waiting, 
    i = 0;
    do{
	    sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) sa, sizeof(*sa)); 
	    packets_waiting = input_timeout(sockfd, TIMEOUT_DURATION);

	    i+=1;
    } while(packets_waiting == 0 && i < NUM_TIMEOUTS);
    if(i == NUM_TIMEOUTS){
    	printf("TIMEOUT ERROR\n");
    	//NEED TO DO ERROR CHECKING
    }
    free(byte_stream);
    
    n = recvWholeStream(sockfd, (char **)&byte_stream, sa);
    resp = marshal.unmarshalMessage((unsigned char *)byte_stream, &resp_len);
    return resp;
}

//we must assume that cliaddr has been wiped clean for us
//this just recives and marshalles it up
Message Sender::recvMessage(int sockfd,  struct sockaddr_in * sa){
    char * buf;
    int len, n; 
    uint call_len; 
    Message m;

    len = sizeof(*sa);
    n = recvWholeStream(sockfd, &buf, sa);
    m = marshal.unmarshalMessage((unsigned char *)buf, &call_len);
    free(buf);

    return m;
}

//we must assume that cliaddr has been preset for us
int Sender::sendResponse(Message m, int sockfd,  struct sockaddr_in * sa){
    char * buf;
    unsigned char * byte_stream;
    int len, n; 
    uint stream_len;

    //first we update the message num
    m.setNum(getUpdateNum());

    len = sizeof(*sa);
    byte_stream = marshal.marshalMessage(m, &stream_len);
    sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) sa, len); 
    free(byte_stream);

    return 0;
}

int Sender::recvWholeStream(int sockfd, char ** buf, struct sockaddr_in * sa){
	int len, n, i;
	uint byte_len;
	unsigned char small_buf [INT_SIZE];
	char * temp_buf;
	len = sizeof(*sa);

	//first we peek and obtain the byte len
	n = recvfrom(sockfd, (char *)small_buf, INT_SIZE, MSG_PEEK | MSG_WAITALL, ( struct sockaddr *) sa, (socklen_t *)&len); 
	byte_len = marshal.getMarshalLength((unsigned char *)small_buf);

	temp_buf = (char *) malloc(byte_len);

	//now read the whole packet
	n = recvfrom(sockfd, (char *)temp_buf, byte_len, MSG_WAITALL, ( struct sockaddr *) sa, (socklen_t *)&len);
	*buf = temp_buf;
	return n;
}

