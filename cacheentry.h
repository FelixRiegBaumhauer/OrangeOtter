#ifndef CACHE_ENTRY_H
#define CACHE_ENTRY_H


#include <string>
#include <iostream>
#include <vector>
#include <ctime>



class CacheEntry{
public:

	/*
	A Cache entry is just a container for a filepath and the last validation time
	*/

	time_t lastValidation;
	std::string filepath;

	CacheEntry(time_t lastValidation, std::string filepath);
	void print();

};



#endif