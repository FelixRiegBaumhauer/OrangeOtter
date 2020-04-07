
#include "cache.h"


void Cache::updateNum(){
    num++;
}

uint Cache::getNum(){
    return num;
}

Cache::Cache(){
	num = 10000;
	this->t = 0;
}

Cache::Cache(uint t){
	num = 10000;
	this->t = t;
}

void Cache::add(std::string filepath, struct sockaddr_in servaddr, int sockfd){
	std::string bytes;
	Message call, resp;


	//need to check that it isnt there already
	//add to the map
	CacheEntry ce = CacheEntry(time(0), filepath);
	cacheMap.push_back(ce);
	
	//then copy over the file 
	//call = Message();
	call = Message(Call, Dump, getNum(), {}, {filepath});
	updateNum();

	resp = sender.sendMessage(call, sockfd, &servaddr);

	bytes = resp.strArgs[0];

	//overwrite the file 
	fs.insertFile(filepath, 0, bytes);


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
	//call = Message();

	call = Message(Call, Dump, getNum(), {}, {filepath});
	updateNum();

	resp = sender.sendMessage(call, sockfd, &servaddr);
	bytes = resp.strArgs[0];
	//overwrite the file 
	fs.insertFile(filepath, 0, bytes);

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


	//first check the T vs the t
	CacheEntry ce = findOrMake(filepath, servaddr, sockfd);

	if(cur_time - ce.lastValidation < t){
		//cache is valid
		return;
	}


	t_client = fs.lastModification(filepath);
	//time_call = Message(); 
	time_call = Message(Call, Fresh, getNum(), {}, {filepath});
	updateNum();

	time_resp = sender.sendMessage(time_call, sockfd, &servaddr);
	t_server = time_resp.intArgs[0];

	if(t_client < t_server){
		std::string bytes;

		//the cache entry is invalid
		//dump_call = Message();
		dump_call = Message(Call, Dump, getNum(), {}, {filepath});
		updateNum();

		dump_resp = sender.sendMessage(dump_call, sockfd, &servaddr);
		bytes = dump_resp.strArgs[0];
		//overwrite the file 
		fs.insertFile(filepath, 0, bytes);
	}	

	//now we update our lastValidation value
	updateCacheTime(filepath, time(0));
}