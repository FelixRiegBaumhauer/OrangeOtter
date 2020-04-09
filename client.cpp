#include "client.h"


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

    //need to send a special end monitor message
    Message endCall = Message(Call, MonitorEnd, {}, {});
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
            int offset, num;
            std::string resStr;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;

            std::cout << "Input the offset position" << std::endl;
            std::cin >> toffset;
            while(toffset < 0){
                std::cout << "No Negative Input, Input the offset position" << std::endl;
                std::cin >> toffset;
            }
            
            std::cout << "Input Number of Characters" << std::endl;
            std::cin >> tnum;
            while(tnum < 0){
                std::cout << "No Negative Input, Input Number of Characters" << std::endl;
                std::cin >> tnum;
            }

            std::cout <<  "read" << std::endl; 
            m = Message(Call, Read, {(uint)offset, (uint)num}, {filepath});
        }

        if(inputText.compare("insert") == 0){
            std::string filepath;
            int offset;
            std::string bytes;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;
            std::cout << "Input the offset position" << std::endl;
            std::cin >> offset;
            while(offset < 0){
                std::cout << "No Negative Input, Input the offset position" << std::endl;
                std::cin >> offset;
            }

            std::cout << "Input Text" << std::endl;
            std::cin >> bytes;

            std::cout <<  "insert" << std::endl;
            m = Message(Call, Insert, {(uint)offset}, {filepath, bytes});
        }

        if(inputText.compare("monitor") == 0){
            std::string filepath;
            int duration;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;
            std::cout << "Input the duration" << std::endl;
            std::cin >> duration;
            while(duration < 0){
                std::cout << "No Negative Input, Input the duration" << std::endl;
                std::cin >> duration;
            }

            std::cout << "monitor" << std::endl;
            m = Message(Call, Monitor, {(uint)duration}, {filepath});
        }

        if(inputText.compare("mode") == 0){
            std::string filepath;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;

            std::cout <<  "mode" << std::endl;
            m = Message(Call, Mode, {}, {filepath});
        }

        if(inputText.compare("shift") == 0){
            std::string filepath;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;
            /*
            std::cout << "Input the direction" << std::endl;
            std::cin >> direction;
            */

            std::cout <<  "shift" << std::endl;
            m = Message(Call, Shift, {1}, {filepath});
        }



        if(inputText.compare("q") == 0){
          std::cout << "Exiting Program" << std::endl;
          return 0;
        }
        
        //if read ie a read call or a mode call we update the cache and then we always read locally
        if(m.callType == Read){
            try{
                cache.updateCache(m.strArgs[0], servaddr, sockfd);

                std::string result;
                result = fs.readFile(m.strArgs[0], m.intArgs[0], m.intArgs[1]);          
                m = Message(Response, Read, 0, {Good}, {result});
            } catch(noFileException e) { m = Message(Response, Read, 0, {Failure, NoFileException}, {});                
            } catch(fileBoundException e){ m = Message(Response, Read, 0, {Failure, FileBoundException}, {});
            }

/*
            //now our cache is updated
            std::string result;
            try{
                result = fs.readFile(m.strArgs[0], m.intArgs[0], m.intArgs[1]);          
                m = Message(Response, Read, 0, {Good}, {result});
            } catch(noFileException e) { m = Message(Response, Read, 0, {Failure, NoFileException}, {});                
            } catch(fileBoundException e){ m = Message(Response, Read, 0, {Failure, FileBoundException}, {});
            }
            */

        }
        else{
            m = sender.sendMessage(m, sockfd, &servaddr);
        }

        processResponse(m, sockfd, &servaddr);



    }
}

Client::Client(){
    this->mode = DEFAULT_CLIENT_MODE;
    this->dropProb = DEFAULT_PROB;
    this->t = DEFAULT_T;

    cache = Cache(DEFAULT_T, &sender);
}

//nasty bug in the making
Client::Client(ClientMode mode, float dropProb, uint t){

    if(mode == DroppingClient){
        this->sender = Sender(dropProb);
    }

    this->mode = mode;
    this->dropProb = dropProb;
    this->t = t;

    this->cache = Cache(t, &sender);
}


// Driver code 
int main(int argc, char ** argv) { 
    int clientPort, serverPort;
    std::string clientIpStr, serverIpStr;
    in_addr_t clientIp, serverIp;
    const char * serverIpPtr;

    printf("This is the Client\n");

    float prob = DEFAULT_PROB;
    ClientMode mode = DEFAULT_CLIENT_MODE;
    uint t = DEFAULT_T;

    int opt;
    while((opt = getopt(argc, argv, "d:t:")) != -1){
        switch(opt){
            case 'd':
                mode = DroppingClient;
                prob = atof(optarg);
                break;
            case 't':
                t = atoi(optarg);
                break;
            default:
                printf("MISTAKE\n");
                break;
        }
    }

    printf("mode: %d\n", mode);
    printf("prob: %f\n", prob);
    printf("t: %d\n", t);


    Client client = Client(mode, prob, t);

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