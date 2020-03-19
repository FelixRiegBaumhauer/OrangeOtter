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

int server_loop(int port){
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr; 
    unsigned char * byte_stream;
    Message m;
    Sender sender;
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { printf("ERROR\n"); return 1; } 
    sender.populateRemoteSockAddr(&servaddr, "127.0.0.1", port);  

    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){ printf("ERROR\n"); return 1; } 

    while(1){
        //clear out old client info        
        memset(&cliaddr, 0, sizeof(cliaddr));

        sender.recvMessage(&m, sockfd, &cliaddr);

        //now we act on the message
        m.print();

        //after we act on the message we send a return
        m  = Message(Response, Read, {}, {"ABCDEF"});

        sender.sendMessage(m, sockfd, &cliaddr);

    }
    return 0; 
}


// Driver code 
int main() { 

    printf("This is the Server\n");

    server_loop(8080);
} 