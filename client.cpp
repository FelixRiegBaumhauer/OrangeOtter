#include "client.h"
  
#define MAXLINE 1024 
  
/*
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
*/


int client_send(int port){
    return 0;
}

int client_loop(int port){

    std::string inputText;
    unsigned char * byte_stream;
    Marshal m;


    int sockfd; 
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    short client_port;

    std::cout << "Establishing Connection" << std::endl;


    client_port = 8081;
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

        Call c = Call(Read, {offset, num}, {filepath});
        c.print();

        uint stream_len;
        byte_stream = m.marshalCall(c, &stream_len);

        //now send it out

        sendto(sockfd, (reinterpret_cast<const char*>(byte_stream)), stream_len, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
        printf("Message sent to server\n"); 
        free(byte_stream);

        int n,len;

        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, (socklen_t *) &len); 
        //buffer[n] = '\0'; 

        uint call_len;
        Response r = m.unmarshalResponse((unsigned char *)buffer, &call_len);
        r.print();

        //printf("Server : %s\n", buffer);


    }

    if(inputText.compare("insert") == 0){
        std::string filepath;
        std::string offset;
        std::string bytes;

        std::cout << "Input Filepath" << std::endl;
        std::cin >> filepath;
        std::cout << "Input the offset position" << std::endl;
        std::cin >> offset;
        std::cout << "Input Text" << std::endl;
        std::cin >> bytes;

        std::cout <<  "insert" << std::endl;

        //fs.insertFile(filepath, std::stoi(offset), bytes);
    }

    if(inputText.compare("monitor") == 0){
        std::string filepath;
        std::string duration;
        std::string clientId;

        std::cout << "Input Filepath" << std::endl;
        std::cin >> filepath;
        std::cout << "Input the duration" << std::endl;
        std::cin >> duration;
        std::cout << "Input clientId" << std::endl;
        std::cin >> clientId;

        std::cout << "monitor" << std::endl;

        //fs.monitorFile(filepath, std::stoi(duration), std::stoi(clientId));
    }

    if(inputText.compare("mode") == 0){
        std::string filepath;
        char c;

        std::cout << "Input Filepath" << std::endl;
        std::cin >> filepath;

        std::cout <<  "mode" << std::endl;

        //c = fs.getMode(filepath);
        //std::cout << "The Most common letter is: " << c << std::endl;
    }

    if(inputText.compare("shift") == 0){
        std::string filepath;
        std::string direction;

        std::cout << "Input Filepath" << std::endl;
        std::cin >> filepath;
        std::cout << "Input the direction" << std::endl;
        std::cin >> direction;

        std::cout <<  "shift" << std::endl;

        //fs.shiftFile(filepath, std::stoi(direction));
    }



    if(inputText.compare("q") == 0){
      std::cout << "Exiting Program" << std::endl;
      return 0;
    }
    }
}


// Driver code 
int main() { 

    printf("This is the Client\n");

    client_loop(8080);
} 