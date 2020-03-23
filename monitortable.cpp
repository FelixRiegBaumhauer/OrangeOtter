
/*
#include <iostream>
#include <string.h>

#include "monitortable.h"

MonitorTable::MonitorTable(){
  length = 0;
  head = nullptr;
}

MonitorTable::~MonitorTable(){
  std::cout << "Destroy MonitorTable" << std::endl;
}

void MonitorTable::insert(MonitorEntry * me){
  me->next = head;
  head = me;
  length+=1;

  std::cout << "Adding To Monitor Table" << std::endl;
  std::cout << me->next << std::endl;
  std::cout << head->next << std::endl;

  std::cout << me << std::endl;
}

//void MonitorTable::find(char * fp){
void MonitorTable::find(std::string fp){
  MonitorEntry * cursor = head;

  while(cursor != nullptr){
    //if(strcmp(cursor->filepath, fp) == 0){
    if( fp.compare(cursor->filepath) == 0 ){
      std::cout << "Found" << cursor->clientId << std::endl;
      return;
    }
    cursor = cursor->next;
  }

  std::cout << "Not Found" << std::endl;
  return;
}

void MonitorTable::print(){

  std::cout << "PRINT" << std::endl;

  MonitorEntry* cursor = head;

  while(cursor != nullptr){
      
    //std::cout << cursor->next << std::endl;
    std::cout << cursor << std::endl;

    //(*cursor).print();
    //std::cout << cursor->filepath << std::endl;
    cursor = cursor->next;
  }

}

//void MonitorTable::findAll(char *fp, std::vector<std::string> * vec){
void MonitorTable::findAll(std::string fp, std::vector<MonitorEntry> * vec){

  MonitorEntry * cursor = head;

  while(cursor != nullptr){
    //if(strcmp(cursor->filepath, fp) == 0){
    if( fp.compare(cursor->filepath) == 0 ){
      //std::string s (cursor->filepath);
      //vec->push_back(s);

      //MonitorEntry new_me = MonitorEntry(cursor);
      //vec->push_back(new_me);
      vec->push_back(*cursor);
    }
    cursor = cursor->next;
  }
  return;
}

std::vector<MonitorEntry> MonitorTable::findAll(std::string fp){
  MonitorEntry * cursor = head;
  std::vector<MonitorEntry> vec;

  while(cursor != nullptr){
    if( fp.compare(cursor->filepath) == 0 ){
      vec.push_back(*cursor);
    }
    cursor = cursor->next;
  }
  return vec; 
}
*/




#include <iostream>
#include <string.h>

#include "monitortable.h"

MonitorTable::MonitorTable(){
}

MonitorTable::~MonitorTable(){
  //std::cout << "Destroy MonitorTable" << std::endl;
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


