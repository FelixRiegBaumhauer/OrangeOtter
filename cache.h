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

	/* Constructors */
	Cache();
	Cache(Sender * sender);
	Cache(uint t, Sender * sender);

	/* Either finds a cache entry or makes one, by contacting the server and copying over the file */
	CacheEntry findOrMake(std::string filepath, struct sockaddr_in servaddr, int sockfd); 

	/* 
	Update the cache entries in our cache map, this is where the client side caching algorithm is implemented 
	First we check if we are witin our t window, if we are no action needed
	Afterwards we poll the server and get the last time of alteration and compare to our time,
	If there is a difernce we overwrite the file in our cache
	Now the cache is appropriately updated
	*/
	void updateCache(std::string filepath, struct sockaddr_in servaddr, int sockfd);

	/* Updates the cache entries time field, if no entry 1 is returned ie error */
	int updateCacheTime(std::string filepath, time_t new_time);
};


#endif