
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

	CacheEntry ce = CacheEntry(time(0), filepath);
	cacheMap.push_back(ce);
	
	//then copy over the file
	call = Message(Call, Dump, {}, {filepath});


	resp = sender->sendMessage(call, sockfd, &servaddr);

	printf("aa\n");
	if(resp.strArgs.size() < 1 || (resp.intArgs.size() > 0 && resp.intArgs[0] == Failure) ){ throw noFileException(); }
	printf("ab\n");
	bytes = resp.strArgs[0];
	//overwrite the file or creates it
	fs.overwriteFile(filepath, bytes);

	return ce;
}

//return 0 if good, 1 if bad
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

//1 if valid, 0 if not
void Cache::updateCache(std::string filepath, struct sockaddr_in servaddr, int sockfd){
	time_t cur_time, t_client, t_server;
	Message time_call, time_resp, dump_call, dump_resp;

	cur_time = time(0);

	printf("a\n");
	//first check the T vs the t
	CacheEntry ce = findOrMake(filepath, servaddr, sockfd);
	printf("b\n");


	if(cur_time - ce.lastValidation < t){ return; /*cache is valid*/ }

	t_client = fs.lastModification(filepath);
	time_call = Message(Call, Fresh, {}, {filepath}); 

	printf("c\n");
	time_resp = sender->sendMessage(time_call, sockfd, &servaddr);
	if(time_resp.strArgs.size() < 1 || (time_resp.intArgs.size() > 0 && time_resp.intArgs[0] == Failure) ){ throw noFileException(); }
	t_server = time_resp.intArgs[0];
	printf("d\n");

	if(t_client < t_server){
		std::string bytes;

		//the cache entry is invalid
		dump_call = Message(Call, Dump, {}, {filepath});


		dump_resp = sender->sendMessage(dump_call, sockfd, &servaddr);
		if(dump_resp.strArgs.size() < 1 || (dump_resp.intArgs.size() > 0 && dump_resp.intArgs[0] == Failure) ){ throw noFileException(); }
		bytes = dump_resp.strArgs[0];
		
		//overwrite the file 
		fs.overwriteFile(filepath, bytes);
	}	
	//now we update our lastValidation value
	updateCacheTime(filepath, time(0));
}