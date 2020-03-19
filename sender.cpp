
#include "sender.h"


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
    int n,len;

    char buffer [MAXLINE];
    Message ack;


    byte_stream = marshal.marshalMessage(m, &stream_len);

    //now send it out
    sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) sa, sizeof(*sa)); 
    free(byte_stream);

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
    char buffer[MAXLINE]; 
    unsigned char * byte_stream;
    int len, n;
    uint stream_len, call_len;
    Message ack;

    struct sockaddr_in cliaddr1;
    //memset(&cliaddr, 0, sizeof(cliaddr));

    len = sizeof(*sa);

	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) sa, (socklen_t *)&len); 

    *m = marshal.unmarshalMessage((unsigned char *)buffer, &call_len);

    ack = Message(AckType, Ack, {}, {});
    byte_stream = marshal.marshalMessage(ack, &stream_len);


    sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) sa, len); 
    free(byte_stream);

    return 0;
}

