#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/time.h>


#include "server.h"
  
#define PORT     8080 
#define MAXLINE 1024 
  
int input_timeout (int filedes, unsigned int seconds)
{
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
    }
    return n;

}

/*
int server_recv(int port){
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { printf("ERROR\n"); return 1; } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY; 
    servaddr.sin_port = htons(port); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){ printf("ERROR\n"); return 1; } 
      
    int len, n; 
  
    len = sizeof(cliaddr);  //len is value/resuslt 


    printf("%d\n", sockfd);


    input_timeout(sockfd, 10);
  
    printf("s_addr: %d\n", cliaddr.sin_addr.s_addr);
    printf("s_port: %d\n", ntohs(cliaddr.sin_port));

    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t *)&len); 
    buffer[n] = '\0'; 

    printf("s_addr: %d\n", cliaddr.sin_addr.s_addr);
    printf("s_port: %d\n", ntohs(cliaddr.sin_port));

    printf("Client : %s\n", buffer); 
    sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
    printf("Hello message sent.\n");  
      
    return 0; 
}
*/


int server_recv(int port){
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 

    unsigned char * byte_stream;


    Marshal m;
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { printf("ERROR\n"); return 1; } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY; 
    servaddr.sin_port = htons(port); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){ printf("ERROR\n"); return 1; } 


    int len, n; 
  
    len = sizeof(cliaddr);  //len is value/resuslt 

    while(1){
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t *)&len); 
        //buffer[n] = '\0'; 

        uint call_len;
        Call c = m.unmarshalCall((unsigned char *)buffer, &call_len);
        c.print();

        //now we send our response
        Response r = Response(Read, Good, "abcdefghij");
        uint stream_len;
        byte_stream = m.marshalResponse(r, &stream_len);

        sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
        printf("Message sent to client.\n");  
        free(byte_stream);
    }

/*
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t *)&len); 
    //buffer[n] = '\0'; 

    uint call_len;
    Call c = m.unmarshalCall((unsigned char *)buffer, &call_len);
    c.print();

    //now we send our response
    Response r = Response(Read, Good, "abcdefghij");
    uint stream_len;
    byte_stream = m.marshalResponse(r, &stream_len);

    sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
    printf("Message sent to client.\n");  
    free(byte_stream);
      */
    return 0; 
}


// Driver code 
int main() { 

    printf("This is the Server\n");

    server_recv(8080);
} 