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
  


void Server::updateNum(){
    num++;
}
uint Server::getNum(){
    return num;
}

//this function has to execute the needed call
Message Server::execute(Message call){
    Message resp;
    return resp;
}

  
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

//return 1 if true, ie we have a collision
int Server::checkMap(Message m, struct sockaddr_in cliaddr){
    uint i = 0;
    MessageEntry mEntry = MessageEntry((uint) cliaddr.sin_port, cliaddr.sin_addr.s_addr, m.num);

    for(i=0; i<messageMap.size(); i++){
        if( mEntry.compareTo(messageMap[i]) == 1){
            return 1;
        }
    }

    //in this case we had no hit, so we add it
    messageMap.push_back(mEntry);

    return 0;
}


int Server::server_loop(int port){
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr; 
    unsigned char * byte_stream;
    Message m;
    Sender sender;
    uint clientNum; //this number is the current client that we are serving


      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { printf("ERROR\n"); return 1; } 
    sender.populateRemoteSockAddr(&servaddr, "127.0.0.1", port);  

    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){ printf("ERROR\n"); return 1; } 

    while(1){
        //check if this message is in our map, then we either discard or proceed

        memset(&cliaddr, 0, sizeof(cliaddr));
        m = sender.recvMessage(sockfd, &cliaddr);

        clientNum = clientMap.findClientNum(cliaddr);

        printf("Serving Client: %d\n", clientNum);

        if( checkMap(m, cliaddr)  == 0){

            m.print();
            //after we act on the message we send a return
            m  = Message(Response, Read, getNum(), {}, {"ABCDEF"});
            updateNum();
            sender.sendResponse(m, sockfd, &cliaddr);
        }else{
            printf("DUPPLICATE PACKET\n");
        }

    }
    return 0; 
}

Server::Server(){
    num = 0;
}


// Driver code 
int main() { 

    Server server;

    printf("This is the Server\n");

    server.server_loop(8080);
} 