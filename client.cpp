#include "client.h"



void Client::updateNum(){
    num++;
}

uint Client::getNum(){
    return num;
}

int Client::client_loop(int server_port, int client_port){
    std::string inputText;

    unsigned char * byte_stream;
    Sender sender;
    Message m;
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr, dummyaddr;


    std::cout << "Establishing Connection" << std::endl;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { printf("ERROR\n"); return 1; } 
    sender.populateRemoteSockAddr(&servaddr, "127.0.0.1", server_port);
    sender.populateRemoteSockAddr(&cliaddr, "127.0.0.1", client_port);

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
            std::cout << "Input clientId" << std::endl;
            std::cin >> clientId;

            std::cout << "monitor" << std::endl;

            m = Message(Call, Monitor, getNum(), {duration, clientId}, {filepath});
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

/*
        sender.sendMessage(m, sockfd, &servaddr);

        //now we wait for our response

        memset(&dummyaddr, 0, sizeof(cliaddr));
        sender.recvMessage(&m, sockfd, &cliaddr);
*/
        m = sender.sendMessage(m, sockfd, &servaddr);
        //and we return our response to the screen
        m.print();



    }
}

Client::Client(){
    num = 0;
}


// Driver code 
int main() { 

    Client client;

    printf("This is the Client\n");

    client.client_loop(8080, 8081);
} 