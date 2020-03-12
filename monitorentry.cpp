/*
#include "monitorentry.h"

#include <string.h>
#include <iostream>
#include <stdlib.h>



MonitorEntry::MonitorEntry(){
  filepath = 0;
  start_time = 0;
  duration = 0;
  client_id = 0;
  next = nullptr; //(MonitorEntry *) 0;
}
 
//
MonitorEntry::~MonitorEntry(){
  std::cout << "start free" << std::endl;
  std::cout << filepath << std::endl;
  if(filepath != 0){
    free(filepath);
  }
  filepath = 0;
  std::cout << "end free" << std::endl;
}
//

MonitorEntry::MonitorEntry(char * fp, int st, int d, int cid){
  int len = strlen(fp);
  filepath = (char *) malloc(len+1);
  strncpy(filepath, fp, len+1);


  //filepath = fp;
  start_time = st;
  duration = d;
  client_id = cid;
  next = nullptr; //(MonitorEntry *) 0;
}

void MonitorEntry::print(){
  std::cout << filepath << ":" << start_time << ":" << duration << ":" << client_id << std::endl;
}
*/


#include "monitorentry.h"

#include <iostream>
#include <string.h>

MonitorEntry::MonitorEntry(){
  //filepath = nullptr;
  filepath = "";
  startTime = -1;
  duration = -1;
  clientId = -1;
  next = nullptr;

  std::cout << "Create Empty" << std::endl;
}

//MonitorEntry::MonitorEntry(char * fp, int s, int d, int c){
MonitorEntry::MonitorEntry(std::string fp, int s, int d, int c){

/*
  int length = strlen(fp);
  filepath = (char *)malloc(length+1);
  strncpy(filepath, fp, length);
  filepath[length] = 0;
  */
  filepath = fp;

  startTime = s;
  duration = d;
  clientId = c;

  next = nullptr;
}

MonitorEntry::MonitorEntry(MonitorEntry * me){
  /*
  int length = strlen(me->filepath);
  filepath = (char *)malloc(length+1);
  strncpy(filepath, me->filepath, length);
  filepath[length] = 0;
  */

  filepath = me->filepath;

  startTime = me->startTime;
  duration = me->duration;
  clientId = me->clientId;

  next = nullptr;
}

MonitorEntry::~MonitorEntry(){
  /*
  if(filepath != nullptr){
    free(filepath);
    filepath = nullptr;
  }
  */
  std::cout << "Destroy" << filepath << std::endl;
}


void MonitorEntry::print(){
  std::cout << "Filepath: " << filepath << " Start Time: " << startTime << " Duration: " << duration <<" Client Id: " << clientId << std::endl;
}

