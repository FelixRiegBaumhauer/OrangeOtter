
#include "clientmap.h"

ClientMap::ClientMap(){
	curNum = 0;

}

uint ClientMap::findClientNum(struct sockaddr_in cliaddr){
	//either we find it and return it or we bump it up one adnd return that
	uint i;
	uint newClientNum;

	for(i=0; i<map.size(); i++){
		if(map[i].find(cliaddr) == 1){
			return map[i].clientNum;
		}
	}

	newClientNum = curNum;
	ClientEntry ce = ClientEntry((uint) cliaddr.sin_port, cliaddr.sin_addr.s_addr, newClientNum);
	map.push_back(ce);

	curNum++;
	return newClientNum;
}

ClientEntry ClientMap::getClientEntry(uint clientNum){
	uint i;

	for(i=0; i<map.size(); i++){
		if(map[i].clientNum == clientNum){
			return map[i];
		}
	}

	throw generalException();
}