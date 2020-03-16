#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 


#include "client.h"
  
#define MAXLINE 1024 
  

int client_send(int port){
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from client"; 
    struct sockaddr_in servaddr, cliaddr;

    short client_port = 8081;
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { printf("ERROR\n"); return 1; } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY; 
    servaddr.sin_port = htons(port); 


    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    cliaddr.sin_family    = AF_INET; // IPv4 
    cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY; 
    cliaddr.sin_port = htons(client_port); 
      
    // Bind the socket with the client address 
    if ( bind(sockfd, (const struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0 ){ printf("ERROR\n"); return 1; } 


    int n, len; 
      
    sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    printf("Hello message sent.\n"); 
          
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, (socklen_t *) &len); 
    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer); 
  
    close(sockfd); 
    return 0; 
}

// Driver code 
int main() { 

    printf("This is the Client\n");

    client_send(8080);
} 