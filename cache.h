#ifndef CACHE_H
#define CACHE_H


#include <string>
#include <iostream>
#include <vector>
#include <ctime>

#include "cacheentry.h"
#include "filesystem.h"
#include "sender.h"
#include "marshal.h"

#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 


class Cache{
public:
	uint t;
	Sender *sender;
	FileSystem fs;
	std::vector<CacheEntry> cacheMap;

	/*
	uint num;
	void updateNum();
	uint getNum();
	*/

	Cache();
	Cache(Sender * sender);
	Cache(uint t, Sender * sender);
	void add(std::string filepath, struct sockaddr_in servaddr, int sockfd);
	CacheEntry findOrMake(std::string filepath, struct sockaddr_in servaddr, int sockfd);
	void updateCache(std::string filepath, struct sockaddr_in servaddr, int sockfd);
	int updateCacheTime(std::string filepath, time_t new_time);


};



#endif