#ifndef CACHE_ENTRY_H
#define CACHE_ENTRY_H


#include <string>
#include <iostream>
#include <vector>
#include <ctime>



class CacheEntry{
public:

	time_t lastValidation;
	std::string filepath;

	CacheEntry(time_t lastValidation, std::string filepath);
	void print();

};



#endif