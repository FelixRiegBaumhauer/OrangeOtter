#include <iostream>
#include <string.h>

#include "monitortable.h"

MonitorTable::MonitorTable(){
}

MonitorTable::~MonitorTable(){
}

void MonitorTable::insert(MonitorEntry me){
  list.push_back(me);
}

void MonitorTable::print(){
  int i;
  for(i=0; i<list.size(); i++){
    std::cout << i+1 << ") " << list[i].toString() << std::endl;
  }
}

std::vector<MonitorEntry> MonitorTable::findAll(std::string fp){
  std::vector<MonitorEntry> vec;  
  int i;
  for(i=0; i<list.size(); i++){
    if( fp.compare(list[i].filepath) == 0 ){
      vec.push_back(list[i]);
    }
  }
  return vec;
}

uint MonitorTable::findRemove(uint clientId){
  std::vector<MonitorEntry> vec;  
  int i, cnt;

  cnt = 0;
  time_t cur_time = time(0);

  for(i=0; i<list.size(); i++){
    if(clientId == list[i].clientId ){
      std::cout << "Take Out: " + list[i].toString() << std::endl;
      list.erase(list.begin() + i);
      cnt++;
    }
  }

  return cnt;
}

std::vector<MonitorEntry> MonitorTable::sweep(std::string fp){
  std::vector<MonitorEntry> vec;  
  int i;

  time_t cur_time = time(0);

  for(i=0; i<list.size(); i++){
    if( fp.compare(list[i].filepath) == 0 ){
      vec.push_back(list[i]);
    }
  }

  return vec;
}

std::vector<MonitorEntry> MonitorTable::sweepClean(std::string fp){
  std::vector<MonitorEntry> vec;  
  int i;

  time_t cur_time = time(0);

  for(i=0; i<list.size(); i++){
    if(list[i].startTime+list[i].duration < cur_time ){
      std::cout << "Take Out: " + list[i].toString() << std::endl;
      list.erase(list.begin() + i);
    }
    else if( fp.compare(list[i].filepath) == 0 ){
      vec.push_back(list[i]);
    }
  }

  return vec;
}


