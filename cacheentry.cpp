
#include "cacheentry.h"


CacheEntry::CacheEntry(time_t lastValidation, std::string filepath){
	this->lastValidation = lastValidation;
	this->filepath = filepath;
}

void CacheEntry::print(){
	std::cout << "Filepath: " << filepath << " T: "<< lastValidation << std::endl;
}