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
    messageNum = 0;
    this->dropProb = 0;
    senderMode = NormalSender;
    srand(time(0));
}

Sender::Sender(float dropProb){
    messageNum = 0;
    this->dropProb = dropProb;
    senderMode = DroppingSender;
    srand(time(0));
}

//1 means yes, do drop and 0 means no dont drop
uint Sender::toDrop(){
    uint r = rand() % 100;
    uint drop = dropProb * 100;

    //printf("r: %d, dp: %d \n", r, drop);

    if(r < drop){
        return 1;
    }
    return 0;
}

int Sender::input_timeout (int filedes, unsigned int seconds, unsigned int * remaing_seconds){
    fd_set set;
    struct timeval timeout;
    int n;

    // Initialize the file descriptor set. //
    FD_ZERO (&set);
    FD_SET (filedes, &set);

    // Initialize the timeout data structure. //
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;

    // select returns 0 if timeout, 1 if input available, -1 if error. //
    if((n = select(FD_SETSIZE, &set, NULL, NULL, &timeout)) < 0){
        return -1;
    }

    //return the old time to use for next 
    if(timeout.tv_usec > 0){
        *remaing_seconds = timeout.tv_sec+1;
    }
    else{
        *remaing_seconds = timeout.tv_sec;
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
    unsigned char * call_stream, * resp_stream;
	uint stream_len, resp_len;
    uint remaing_seconds;
    int n, len, packets_waiting, i, j;
    Message resp;

    //first we update the message num
    call.setNum(getUpdateNum());

    //std::cout << "Sent: ";
    //call.print();

    //basic idea is that we first send a call, wait for response and then return that
    //byte_stream = marshal.marshalMessage(call, &stream_len);
    call_stream = marshal.marshalMessage(call, &stream_len);


    //the idea of this do while loop is to check if we have an response waiting, 

    for(i=0; i<NUM_TIMEOUTS; i++){
        //send packet
        if(toDrop() == 0){
            if(senderMode == DroppingSender){ printf("PACKET SENT\n"); }
            sendto(sockfd, (reinterpret_cast<const char*>(call_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) sa, sizeof(*sa)); 
        } else if(senderMode == DroppingSender){ printf("PACKET DROPPED\n"); } 

        remaing_seconds = TIMEOUT_DURATION;

        while(remaing_seconds > 0){

            //poll for arrivals
            packets_waiting = input_timeout(sockfd, remaing_seconds, &remaing_seconds);

            //for all the arrived now we read and marshal
            for(j=0; j<packets_waiting; j++){
                n = recvWholeStream(sockfd, (char **)&resp_stream, sa);
                resp = marshal.unmarshalMessage((unsigned char *)resp_stream, &resp_len);
        
                if(call.num == resp.num && call.callType == resp.callType){ //need to add call.num == resp.num &&
                    free(call_stream);
                    free(resp_stream);
                    
                    //std::cout << "Recivied: ";
                    //resp.print();
                    
                    return resp;
                }
                else{
                    //we can free the response stream as every time this loop happens we re alloc it
                    free(resp_stream);

                    //std::cout << "Recivied: ";
                    //resp.print();
                    
                    printf("IMPROPER PACKET RECIEVED\n");
                }
            }
        }

    }

    free(call_stream);
    throw timeoutException();
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

    //std::cout << "Recivied: ";
    //m.print();

    return m;
}

//we must assume that cliaddr has been preset for us
int Sender::sendResponse(Message call, Message resp, int sockfd,  struct sockaddr_in * sa){
    char * buf;
    unsigned char * byte_stream;
    int len, n; 
    uint stream_len;

    //first we update the message num
    //resp.setNum(getUpdateNum());
    //we set teh response number to the call number so we can filter out on the recieving side
    resp.setNum(call.getNum());

    //std::cout << "Sent: ";
    //resp.print();

    len = sizeof(*sa);
    byte_stream = marshal.marshalMessage(resp, &stream_len);

    if(toDrop() == 0){
        if(senderMode == DroppingSender){ printf("PACKET SENT\n"); }
        sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) sa, len); 
    } else if(senderMode == DroppingSender){ printf("PACKET DROPPED\n"); }
    free(byte_stream);

    return 0;
}

/* recvWHoleStream takes the first 4 bytes, which contain the size, after wehcih we read the whole packet */
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

