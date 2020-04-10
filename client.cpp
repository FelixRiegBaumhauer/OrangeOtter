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
    if((n = select(FD_SETSIZE, &set, NULL, NULL, &timeout)) < 0){ return -1; }
    return n;
}

Message Client::handleMonitor(Message m, int sockfd, struct sockaddr_in * sa){
    uint duration, diff;
    int n;
    time_t start_time, end_time, cur_time;
    Message notification, resp;

    /* Get start and end time*/
    duration = m.intArgs[1];
    start_time = time(0);
    end_time = start_time + duration;

    /* We loop for as long as  */
    while( (cur_time = time(0)) < end_time){
        diff = end_time - cur_time;

        //here we wait for packets to come in
        if( (n = input_timeout(sockfd, diff)) == -1){ throw generalException(); }

        if(n > 0){ /* If valid packet, we can now read the packet and show to user */
            notification = sender.recvMessage(sockfd, sa);
            //notification.print();

            //need to check for errors
            std::cout << "Monitor Update: " << notification.strArgs[0] << " has been altered" << std::endl;
            std::cout "The current state of the file: " << std::endl;
            std::cout << notification.strArgs[1] << std::endl;
        }
    }

    //need to send a special end monitor message that will be acknoldeged
    Message endCall = Message(Call, MonitorEnd, {}, {});
    resp = sender.sendMessage(endCall, sockfd, sa);
    return resp;
}

void Client::processResponse(Message m, int sockfd, struct sockaddr_in * sa){

    //check for errors right here 
    if(m.intArgs.size() == 0){
        throw generalException();
    }

    if(m.intArgs[0] == Good){
        switch(m.callType){
            case Read: 
                //m.print();
                std::cout << m.strArgs[0] << std::endl;
                break;
            case Insert:
                //m.print(); 
                break;
            case Monitor: /* Monitor requires special behavior */
                //m.print();
                std::cout << "Begin Monitoring" << std::endl;
                m = handleMonitor(m, sockfd, sa);
                std::cout << "Monitoring Ended" << std::endl;
                //m.print();
                break;
            case Shift: 
                //m.print();
                std::cout << "File Shifted" << std::endl;
                break;
            case Mode: 
                //m.print();
                std::cout << m.strArgs[0] << std::endl;
                break;
            default: //ie if a monitor update or monitor end was included somehow
                throw improperPacketException();
                break;
        }
    }
    else{ /* This means there was a failure of some sort */
        if(m.intArgs.size() == 2){
            if(m.intArgs[1] == NoFileException){
                std::cout << "The mentioned file does not exist" << std::endl;
            }
            else if(m.intArgs[1] == FileBoundException){
                std::cout << "The Bounds supplied were improper" << std::endl;
            }
            else{
                throw generalException();
            }
        }
        else{
            throw generalException();
        }
    }
}

int Client::client_loop(int server_port, int client_port, in_addr_t server_ip, in_addr_t client_ip){
    std::string inputText;

    unsigned char * byte_stream;
    Message m;
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr, dummyaddr;

    /* seeting up socket and populating sockaddr */
    std::cout << "Establishing Connection" << std::endl;
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { return 1; } 
    sender.populateRemoteSockAddr(&cliaddr, client_ip, client_port);
    sender.populateRemoteSockAddr(&servaddr, server_ip, server_port);
    std::cout << "Connection Established" << std::endl;

    // Bind the socket with the client address 
    if ( bind(sockfd, (const struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0 ){ return 1; } 

    /* This is the loop were we ask for input */
    while(1){
        std::cout << "Enter in your choice" << std::endl;
        std::cin >> inputText;

        if(inputText.compare("read") == 0){
            std::string filepath;
            int offset, num;
            std::string resStr;

            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;

            std::cout << "Input the offset position" << std::endl;
            std::cin >> offset;
            while(offset < 0){
                std::cout << "No Negative Input, Input the offset position" << std::endl;
                std::cin >> offset;
            }
            
            std::cout << "Input Number of Characters" << std::endl;
            std::cin >> num;
            while(num < 0){
                std::cout << "No Negative Input, Input Number of Characters" << std::endl;
                std::cin >> num;
            }

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
            m = Message(Call, Monitor, {(uint)duration}, {filepath});
        }

        if(inputText.compare("mode") == 0){
            std::string filepath;
            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;
            m = Message(Call, Mode, {}, {filepath});
        }

        if(inputText.compare("shift") == 0){
            std::string filepath;
            std::cout << "Input Filepath" << std::endl;
            std::cin >> filepath;
            m = Message(Call, Shift, {1}, {filepath});
        }

        if(inputText.compare("q") == 0){
          std::cout << "Exiting Program" << std::endl;
          return 0;
        }
        
        //if read ie a read call or a mode call we update the cache and then we always read locally
        if(m.callType == Read){
            try{
                /* First update the cache, this we add the file if needed */
                cache.updateCache(m.strArgs[0], servaddr, sockfd);

                std::string result;
                result = fs.readFile(m.strArgs[0], m.intArgs[0], m.intArgs[1]); /* Now we can read*/
                m = Message(Response, Read, 0, {Good}, {result}); /* We make our own response */
            } catch(noFileException e) { m = Message(Response, Read, 0, {Failure, NoFileException}, {});                
            } catch(fileBoundException e){ m = Message(Response, Read, 0, {Failure, FileBoundException}, {});
            }
        }else{ m = sender.sendMessage(m, sockfd, &servaddr); }

        try{
            processResponse(m, sockfd, &servaddr); /* All responses get processed */
        } catch(generalException e){ return 1; }


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

    float prob = DEFAULT_PROB;
    ClientMode mode = DEFAULT_CLIENT_MODE;
    uint t = DEFAULT_T;

    /* Use getopt to read in arguments */
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
                return 1;
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


    std::string modeString = (mode == NormalClient) ? "Normal Client Mode" : "Dropping Client Packtes Mode";
    std::cout << "Running in " << modeString << std::endl;
    printf("Probabibillty of packet drop: %f\n", prob);
    std::cout << "Client Cahing T value: " << t << std::endl;


    try{
        if(client.client_loop(serverPort, clientPort, serverIp, clientIp) != 0){
            std::cout << "Terminated for unknown reasons" << std::endl;
        }
    }
    catch(timeoutException e){
        std::cout << "The Message has timedout too many times, the program will halt" << std::endl;
    } 
} 