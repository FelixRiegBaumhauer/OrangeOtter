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
    (*cursor).print();
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

