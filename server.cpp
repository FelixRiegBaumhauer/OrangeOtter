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
  

void Server::sendList(int sockfd, std::vector<uint> clientNums, std::string filepath){
    uint i;
    struct sockaddr_in cliaddr;
    std::string wholeFile;


    for(i=0; i<clientNums.size(); i++){
        try{
            ClientEntry ce = clientMap.getClientEntry(clientNums[i]); /* If an entry is not found we throw a general exception */

            memset(&cliaddr, 0, sizeof(cliaddr)); 
            cliaddr.sin_family = AF_INET; 
            cliaddr.sin_addr.s_addr = ce.ip_addr;  
            cliaddr.sin_port = ce.port; 

            wholeFile = fs.readWholeFile(filepath);

            Message m = Message(Response, MonitorUpdate, {}, {filepath, wholeFile});
            sender.sendResponse(m, sockfd, &cliaddr);  
        } catch(generalException e){
            std::cout << "Client with id: " << clientNums[i] << " could not be found" << std::endl;
        }      
    }
}

//this function has to execute the needed call
Message Server::execute(int sockfd, Message call, uint clientNum){
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

            try {
                result = fs.readFile(filepath, offset, num);
                //std::cout << "Result: " << result << std::endl;
                
                respType = Response;
                respCallType = Read;
                respIntArgs.push_back(Good);
                respStrArgs.push_back(result);
            } catch(noFileException e) {
                respType = Response;
                respCallType = Read;
                respIntArgs.push_back(Failure);
                respIntArgs.push_back(NoFileException);
            } catch(fileBoundException e) {
                respType = Response;
                respCallType = Read;
                respIntArgs.push_back(Failure);
                respIntArgs.push_back(FileBoundException);
            }
            break;
        }
        case Insert:
        {
            uint offset = call.intArgs[0];
            std::string filepath = call.strArgs[0];
            std::string bytes = call.strArgs[1];

            std::vector<uint> clientNums;

            try{
                clientNums = fs.insertFile(filepath, offset, bytes);

                //send the monitor responses
                sendList(sockfd, clientNums, filepath);

                respType = Response;
                respCallType = Insert;
                respIntArgs.push_back(Good); 
            } catch(fileBoundException e) {
                respType = Response;
                respCallType = Insert;
                respIntArgs.push_back(Failure);
                respIntArgs.push_back(FileBoundException);
            }

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

            try{
                clientNums = fs.shiftFile(filepath, direction);

                //send the monitor responses
                sendList(sockfd, clientNums, filepath);

                respType = Response;
                respCallType = Shift;
                respIntArgs.push_back(Good);
            } catch(noFileException e) {
                respType = Response;
                respCallType = Shift;
                respIntArgs.push_back(Failure);
                respIntArgs.push_back(NoFileException);
            }
            break;
        }
        case Mode:
        {
            std::string filepath = call.strArgs[0];
            char c;
            std::string result;

            try{
                c = fs.getMode(filepath);
                result.push_back(c);
                //std::cout << "Result: " << result << std::endl;

                respType = Response;
                respCallType = Mode;
                respIntArgs.push_back(Good);
                respStrArgs.push_back(result);
            } catch(noFileException e) {
                respType = Response;
                respCallType = Shift;
                respIntArgs.push_back(Failure);
                respIntArgs.push_back(NoFileException);
            }
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

            try{
                result = fs.lastModification(filepath);

                respType = Response;
                respCallType = Fresh;
                respIntArgs.push_back(Good);
                respIntArgs.push_back(result);
            } catch(noFileException e) {
                respType = Response;
                respCallType = Fresh;
                respIntArgs.push_back(Failure);
                respIntArgs.push_back(NoFileException);
            }

            break;
        }
        case Dump:{
            std::string filepath = call.strArgs[0];
            std::string result;

            try{
                result = fs.readWholeFile(filepath);

                respType = Response;
                respCallType = Dump;
                respStrArgs.push_back(result);
            } catch(noFileException e) {
                respType = Response;
                respCallType = Dump;
                respIntArgs.push_back(Failure);
                respIntArgs.push_back(NoFileException);
            }
            break;
        }
        case Start:{
            std::cout << "Starting a new client" << std::endl;
            respType = Response;
            respCallType = Start;
            respIntArgs.push_back(Good);
            break;
        }
    }

    Message resp = Message(respType, respCallType, respIntArgs, respStrArgs);
    //resp.print();
    return resp;
}

//return 1 if true, ie we have a collision
int Server::checkMap(Message m, struct sockaddr_in cliaddr, InvocationSemantic semantic){
    uint i = 0;

    if(semantic == AtLeastOnce){ 
        return 0; 
    } /* No need to check if we use AtLeastOnce */

    //clear out if we have a start
    if(m.callType == Start){
        clearMap(m, cliaddr);
    }

    MessageEntry mEntry = MessageEntry((uint) cliaddr.sin_port, cliaddr.sin_addr.s_addr, m.num, m);
    for(i=0; i<messageMap.size(); i++){
        if( mEntry.compareTo(messageMap[i]) == 1){
            return 1;
        }
    }

    //in this case we had no hit, so we add it
    //messageMap.push_back(mEntry);
    return 0;
}

void Server::clearMap(Message m,  struct sockaddr_in cliaddr){
    int i = 0;

    MessageEntry mEntry = MessageEntry((uint) cliaddr.sin_port, cliaddr.sin_addr.s_addr, m.num, m);

    while(i < messageMap.size()){
        if( mEntry.compareHost(messageMap[i]) == 1){
            messageMap.erase(messageMap.begin() + i);
        }
        else{ i++; }
    }
}

void Server::addMap(Message m, struct sockaddr_in cliaddr, InvocationSemantic semantic, Message resp){
    if(semantic == AtLeastOnce){
        return;
    }

    MessageEntry mEntry = MessageEntry((uint) cliaddr.sin_port, cliaddr.sin_addr.s_addr, m.num, resp);
    messageMap.push_back(mEntry);
}

Message Server::getFromMap(Message m, struct sockaddr_in cliaddr, InvocationSemantic semantic){
    uint i = 0;

    if(semantic == AtLeastOnce){ throw generalException(); } /*This should not happen */

    for(i=0; i<messageMap.size(); i++){
        if( messageMap[i].compareTo((uint) cliaddr.sin_port, cliaddr.sin_addr.s_addr, m.num) == 1){
            return messageMap[i].message;
        }
    }

    //if we find no hit
    throw generalException();
}


int Server::server_loop(int port, in_addr_t serverIp){
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr; 
    unsigned char * byte_stream;
    Message m, resp;
    uint clientNum; //this number is the current client that we are serving


      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { return 1; } 
    sender.populateRemoteSockAddr(&servaddr, serverIp, port);  

    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){ return 1; } 

    while(1){
        //check if this message is in our map, then we either discard or proceed

        memset(&cliaddr, 0, sizeof(cliaddr));
        m = sender.recvMessage(sockfd, &cliaddr);
        clientNum = clientMap.findClientNum(cliaddr);

        if(waitTime > 0){
            std::cout << "Now we sleep for " << waitTime << " seconds, to simulate delays in network" << std::endl;
            sleep(waitTime);
            std::cout << "Server wakes" << std::endl; 
        }

        printf("Serving Client: %d\n", clientNum);
        m.print();

        if( checkMap(m, cliaddr, semantic) == 0){
            //we should change this to a list that we then go through and send out
            resp = execute(sockfd, m, clientNum);
            //now we add to the map
            addMap(m, cliaddr, semantic, resp);
            //after we act on the message we send a return
            sender.sendResponse(resp, sockfd, &cliaddr);
        }else{ 
            printf("DUPLICATE PACKET\n"); 
            resp = getFromMap(m, cliaddr, semantic);
            sender.sendResponse(resp, sockfd, &cliaddr);
        }
    }
    return 0; 
}

Server::Server(){
    this->semantic = DEFAULT_INVOCATION_SEMANTIC;
    this->mode = DEFAULT_SERVER_MODE;
    this->dropProb = DEFAULT_PROB;
    this->waitTime = DEFAULT_WAIT_TIME;
}

Server::Server(InvocationSemantic semantic, ServerMode mode, float dropProb, uint waitTime){

    if(mode == DroppingServer){
        sender = Sender(dropProb);
    }

    this->semantic = semantic;
    this->mode = mode;
    this->dropProb = dropProb;
    this->waitTime = waitTime;
}


// Driver code 
int main(int argc, char ** argv) { 
    int serverPort;
    std::string serverIpStr;
    in_addr_t serverIp;
    const char * serverIpPtr;
    Server server;

    /*
    We either have server -m => at-most-once or server -l => at-least-once
    and we have server -d p where p is the drop percentage for the dropping
    */
    float prob;
    uint waitTime;
    InvocationSemantic semantic;
    ServerMode mode;

    semantic = DEFAULT_INVOCATION_SEMANTIC; //AtMostOnce
    mode = DEFAULT_SERVER_MODE; //NormalServer
    prob = DEFAULT_PROB; //0
    waitTime = DEFAULT_WAIT_TIME; //0


    //need to error proof if both -m and -l passed
    int opt;
    while((opt = getopt(argc, argv, "ld:w:")) != -1){
        switch(opt){
            case 'd':
                mode = DroppingServer;
                prob = atof(optarg);
                break;
            case 'w':
                waitTime = atoi(optarg);
                break;
            case 'l':
                semantic = AtLeastOnce;
                break;
            default:
                std::cout << "Improper arguments" << std::endl;
                return 1;
                break;
        }
    }

    server = Server(semantic, mode, prob, waitTime);

    serverPort = 8080;
    serverIpStr = "127.0.0.1";
    serverIpPtr = serverIpStr.c_str();
    serverIp = inet_addr(serverIpPtr);

    std::cout << "Server hosted on Port: " << serverPort << " and IP address: " << serverIpStr << std::endl;

    std::string modeString = (mode == NormalServer) ? "Normal Server Mode" : "Dropping Server Packtes Mode";
    std::string semanticString = (semantic == AtMostOnce) ? "At-Most-Once" : "At-Least-Once";
    std::cout << "Running in " << modeString << ", Using " << semanticString << " semantics" << std::endl;
    printf("Probabibillty of packet drop: %f, and wait time of: %d\n", prob, waitTime);

    server.server_loop(serverPort, serverIp);

    std::cout << "Server ended Abrubtlly" << std::endl;

    return 0;
} 