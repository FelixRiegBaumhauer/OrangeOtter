#include "client.h"



void Client::updateNum(){
    num++;
}

uint Client::getNum(){
    return num;
}

int Client::input_timeout (int filedes, unsigned int seconds)
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

Message Client::handleMonitor(Message m, int sockfd, struct sockaddr_in * sa){
    uint duration, diff;
    int n;
    time_t start_time, end_time, cur_time;
    Message notification, resp;

    duration = m.intArgs[1];
    start_time = time(0);
    end_time = start_time + duration;

    while( (cur_time = time(0)) < end_time){
        diff = end_time - cur_time;

        //here we wait for packets to come in
        n = input_timeout(sockfd, diff);
        //need to do error checking

        if(n > 0){
            //now read messgae
            notification = sender.recvMessage(sockfd, sa);
            notification.print();
        }
    }

    //need to send a special end monitor messgae
    Message endCall = Message(Call, MonitorEnd, getNum(), {}, {});
    updateNum();
    resp = sender.sendMessage(endCall, sockfd, sa);
    return resp;
}

void Client::processResponse(Message m, int sockfd, struct sockaddr_in * sa){
    //if we have a monitor we need to wait now

    switch(m.callType){
        case Read: 
            m.print();
            break;
        case Insert:
            m.print(); 
            break;
        case Monitor:
            m.print();
            m = handleMonitor(m, sockfd, sa);
            m.print();
            break;
        case Shift: 
            m.print();
            break;
        case Mode: 
            m.print();
            break;
        case MonitorUpdate:
            m.print(); 
            break;
        case MonitorEnd:
            m.print(); 
            break;
        default: 
            break;

    }

}

int Client::client_loop(int server_port, int client_port, in_addr_t server_ip, in_addr_t client_ip){
    std::string inputText;

    unsigned char * byte_stream;
    Message m;
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr, dummyaddr;


    std::cout << "Establishing Connection" << std::endl;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { printf("ERROR\n"); return 1; } 
    sender.populateRemoteSockAddr(&cliaddr, client_ip, client_port);
    sender.populateRemoteSockAddr(&servaddr, server_ip, server_port);

    std::cout << "Connection Established" << std::endl;


    // Bind the socket with the client address 
    if ( bind(sockfd, (const struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0 ){ printf("ERROR\n"); return 1; } 



    while(1){

        std::cout << "Enter in your choice" << std::endl;
        std::cin >> inputText;
        std::cout << "We wrote: " << inputText << std::endl;

        if(inputText.compare("read") == 0){
            std::string filepath;
            uint offset;
            uint num;

            std::string resStr;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;
            std::cout << "Input the offset position" << std::endl;
            std::cin >> offset;
            std::cout << "Input Number of Characters" << std::endl;
            std::cin >> num;

            std::cout <<  "read" << std::endl; 

            m = Message(Call, Read, getNum(), {offset, num}, {filepath});
            updateNum();
        }

        if(inputText.compare("insert") == 0){
            std::string filepath;
            uint offset;
            std::string bytes;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;
            std::cout << "Input the offset position" << std::endl;
            std::cin >> offset;
            std::cout << "Input Text" << std::endl;
            std::cin >> bytes;

            std::cout <<  "insert" << std::endl;

            m = Message(Call, Insert, getNum(), {offset}, {filepath, bytes});
            updateNum();
        }

        if(inputText.compare("monitor") == 0){
            std::string filepath;
            uint duration;
            uint clientId;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;
            std::cout << "Input the duration" << std::endl;
            std::cin >> duration;

            std::cout << "monitor" << std::endl;

            m = Message(Call, Monitor, getNum(), {duration}, {filepath});
            updateNum();
        }

        if(inputText.compare("mode") == 0){
            std::string filepath;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;

            std::cout <<  "mode" << std::endl;

            m = Message(Call, Mode, getNum(), {}, {filepath});
            updateNum();
        }

        if(inputText.compare("shift") == 0){
            std::string filepath;
            uint direction;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;
            std::cout << "Input the direction" << std::endl;
            std::cin >> direction;

            std::cout <<  "shift" << std::endl;

            m = Message(Call, Shift, getNum(), {direction}, {filepath});
            updateNum();
        }



        if(inputText.compare("q") == 0){
          std::cout << "Exiting Program" << std::endl;
          return 0;
        }
        
        m = sender.sendMessage(m, sockfd, &servaddr);

        processResponse(m, sockfd, &servaddr);



    }
}

Client::Client(){
    num = 0;
}


// Driver code 
int main() { 
    int clientPort, serverPort;
    std::string clientIpStr, serverIpStr;
    in_addr_t clientIp, serverIp;
    const char * serverIpPtr;
    Client client;

    printf("This is the Client\n");

    std::cout << "Enter the desired Client Port number" << std::endl;
    std::cin >> clientPort;

    std::cout << "Enter the desired Server IP number" << std::endl;
    std::cin >> serverIpStr;
    serverIpPtr = serverIpStr.c_str();
    serverIp = inet_addr(serverIpPtr);

    std::cout << "Enter the desired Server Port number" << std::endl;
    std::cin >> serverPort;  


    clientIp = inet_addr("127.0.0.1");


    client.client_loop(serverPort, clientPort, serverIp, clientIp);
} 