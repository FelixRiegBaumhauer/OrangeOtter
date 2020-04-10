#include "cache.h"



Cache::Cache(){
	Sender s;
	this->sender = &s;
	this->t = 0;
}

Cache::Cache(Sender * sender){
	this->sender = sender;
	this->t = 0;
}

Cache::Cache(uint t, Sender * sender){
	this->sender = sender;
	this->t = t;
}

CacheEntry Cache::findOrMake(std::string filepath, struct sockaddr_in servaddr, int sockfd){
	Message call, resp;
	std::string bytes;

	uint i;

	for(i=0; i<cacheMap.size(); i++){
		if(cacheMap[i].filepath.compare(filepath) == 0){
			return cacheMap[i];
		}
	}

	//In this case we have not found the entry so we make one

	call = Message(Call, Dump, {}, {filepath});
	resp = sender->sendMessage(call, sockfd, &servaddr);

	if(resp.strArgs.size() < 1 || (resp.intArgs.size() > 0 && resp.intArgs[0] == Failure) ){ throw noFileException(); }

	bytes = resp.strArgs[0];
	fs.overwriteFile(filepath, bytes); /* Overwrite file */

	//we only enter in the entry after a succesfu entry
	CacheEntry ce = CacheEntry(time(0), filepath);
	cacheMap.push_back(ce);

	return ce;
}

//return 0 if we found a cache entry, 1 if error //
int Cache::updateCacheTime(std::string filepath, time_t new_time){
	uint i;

	for(i=0; i<cacheMap.size(); i++){
		if(cacheMap[i].filepath.compare(filepath) == 0){
			cacheMap[i].lastValidation = new_time;
			return 0;
		}
	}
	return 1;
}


void Cache::updateCache(std::string filepath, struct sockaddr_in servaddr, int sockfd){
	time_t cur_time, t_client, t_server;
	Message time_call, time_resp, dump_call, dump_resp;

	cur_time = time(0);
	CacheEntry ce = findOrMake(filepath, servaddr, sockfd); /* compare T vs t */

	if(cur_time - ce.lastValidation < t){ return; /*cache is valid*/ }

	t_client = fs.lastModification(filepath);
	time_call = Message(Call, Fresh, {}, {filepath}); 

	time_resp = sender->sendMessage(time_call, sockfd, &servaddr);
	if(time_resp.intArgs.size() != 2 || (time_resp.intArgs.size() > 0 && time_resp.intArgs[0] == Failure) ){ 
		throw noFileException(); 
	}
	
	t_server = time_resp.intArgs[1];

	if(t_client < t_server){ /* In this case the entry is invalid */
		std::string bytes;

		dump_call = Message(Call, Dump, {}, {filepath});

		dump_resp = sender->sendMessage(dump_call, sockfd, &servaddr);
		if(dump_resp.strArgs.size() < 1 || (dump_resp.intArgs.size() > 0 && dump_resp.intArgs[0] == Failure) ){ 
			throw noFileException(); 
		}
		bytes = dump_resp.strArgs[0];
		fs.overwriteFile(filepath, bytes); /* In this case we overwrite the file */
	}

	updateCacheTime(filepath, time(0)); /* Now updtae our validation value with appropriate new time */
}