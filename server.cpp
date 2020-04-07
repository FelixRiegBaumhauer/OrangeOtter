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
  

/*
void Server::updateNum(){
    num++;
}
uint Server::getNum(){
    return num;
}

uint Server::getUpdateNum(){
    uint tempNum = num;
    num++;
    return tempNum;
}
*/

void Server::sendList(int sockfd, std::vector<uint> clientNums, std::string filepath){
    uint i;
    struct sockaddr_in cliaddr;


    for(i=0; i<clientNums.size(); i++){
        ClientEntry ce = clientMap.getClientEntry(clientNums[i]);

        memset(&cliaddr, 0, sizeof(cliaddr)); 
        cliaddr.sin_family = AF_INET; 
        cliaddr.sin_addr.s_addr = ce.ip_addr;  
        cliaddr.sin_port = ce.port; 

        //Message m = Message(Response, MonitorUpdate, getUpdateNum(), {}, {filepath});
        Message m = Message(Response, MonitorUpdate, {}, {filepath});


        sender.sendResponse(m, sockfd, &cliaddr);        
    }
}

//this function has to execute the needed call
Message Server::execute(int sockfd, Message call, uint clientNum){

    //need to do appropriate checking here
    //for each case we call

    MessageType respType;
    CallType respCallType;
    std::vector<uint> respIntArgs;
    std::vector<std::string> respStrArgs;


    switch(call.callType){
        case Read: 
        {
            uint offset = call.intArgs[0];
            uint num = call.intArgs[1];
            std::string filepath = call.strArgs[0];
            std::string result;

            result = fs.readFile(filepath, offset, num);
            std::cout << "Result: " << result << std::endl;

            respType = Response;
            respCallType = Read;
            respIntArgs.push_back(Good);
            respStrArgs.push_back(result);
            //now we return
            break;
        }
        case Insert:
        {
            uint offset = call.intArgs[0];
            std::string filepath = call.strArgs[0];
            std::string bytes = call.strArgs[1];

            std::vector<uint> clientNums;

            clientNums = fs.insertFile(filepath, offset, bytes);

            //send the monitor responses
            sendList(sockfd, clientNums, filepath);

            respType = Response;
            respCallType = Insert;
            respIntArgs.push_back(Good); //needs to be conditional
            break;
        }
        case Monitor:
        {
            uint duration = call.intArgs[0];
            std::string filepath = call.strArgs[0];

            fs.monitorFile(filepath, duration, clientNum);

            respType = Response;
            respCallType = Monitor;
            respIntArgs.push_back(Good);
            respIntArgs.push_back(duration);
            break;
        }
        case Shift:
        {
            uint direction = call.intArgs[0];
            std::string filepath = call.strArgs[0];
            std::vector<uint> clientNums;

            clientNums = fs.shiftFile(filepath, direction);

            //send the monitor responses
            sendList(sockfd, clientNums, filepath);

            respType = Response;
            respCallType = Shift;
            respIntArgs.push_back(Good);
            break;
        }
        case Mode:
        {
            std::string filepath = call.strArgs[0];
            char c;
            std::string result;

            c = fs.getMode(filepath);
            result.push_back(c);
            std::cout << "Result: " << result << std::endl;

            respType = Response;
            respCallType = Mode;
            respIntArgs.push_back(Good);
            respStrArgs.push_back(result);
            break;
        }
        case MonitorEnd:
        {
            fs.findRemove(clientNum);
            respType = Response;
            respCallType = MonitorEnd;
            break;
        }
        case Fresh:{
            std::string filepath = call.strArgs[0];
            uint result;

            result = fs.lastModification(filepath);

            respType = Response;
            respCallType = Fresh;
            respIntArgs.push_back(result);

            break;
        }
        case Dump:{
            std::string filepath = call.strArgs[0];
            std::string result;

            result = fs.readWholeFile(filepath);

            respType = Response;
            respCallType = Dump;
            respStrArgs.push_back(result);

            break;
        }


    }

    //Message resp = Message(respType, respCallType, getUpdateNum(), respIntArgs, respStrArgs);
    Message resp = Message(respType, respCallType, respIntArgs, respStrArgs);

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


int Server::server_loop(int port, in_addr_t serverIp){
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr; 
    unsigned char * byte_stream;
    Message m;
    uint clientNum; //this number is the current client that we are serving


      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { printf("ERROR\n"); return 1; } 
    sender.populateRemoteSockAddr(&servaddr, serverIp, port);  

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

            //we should change this to a list that we then go through and send out
            m = execute(sockfd, m, clientNum);
            
            //after we act on the message we send a return
            //m  = Message(Response, Read, getNum(), {}, {"ABCDEF"});
            //updateNum();
            sender.sendResponse(m, sockfd, &cliaddr);
        }else{
            printf("DUPPLICATE PACKET\n");
            m.print();
        }

    }
    return 0; 
}

Server::Server(){
    //num = 0;
}


// Driver code 
int main() { 
    int serverPort;
    std::string serverIpStr;
    in_addr_t serverIp;
    const char * serverIpPtr;
    Server server;

    serverPort = 8080;

    printf("This is the Server\n");

    serverIpStr = "127.0.0.1";
    serverIpPtr = serverIpStr.c_str();
    serverIp = inet_addr(serverIpPtr);

    std::cout << "Server hosted on Port: " << serverPort << " and IP address: " << serverIpStr << std::endl;

    server.server_loop(serverPort, serverIp);
} 