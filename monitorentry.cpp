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

/*
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
MonitorEntry::MonitorEntry(std::string fp, time_t s, int d, int c){

  filepath = fp;
  startTime = s;
  duration = d;
  clientId = c;

  next = nullptr;
}

MonitorEntry::MonitorEntry(MonitorEntry * me){

  filepath = me->filepath;
  startTime = me->startTime;
  duration = me->duration;
  clientId = me->clientId;

  next = nullptr;
}

MonitorEntry::~MonitorEntry(){
  std::cout << "Destroy" << filepath << std::endl;
}


void MonitorEntry::print(){
  std::cout << "Filepath: " << filepath << " Start Time: " << startTime << " Duration: " << duration <<" Client Id: " << clientId << std::endl;
}

std::string MonitorEntry::toString(){
  std::string retStr = "Yup";
  //retStr.append("Filepath: ");
  //retStr.push_down(to_string(filepath));
  // << " Start Time: " << startTime << " Duration: " << duration <<" Client Id: " << clientId;
  return retStr;
}
*/



#include "monitorentry.h"

MonitorEntry::MonitorEntry(){
  filepath = "";
  startTime = -1;
  duration = -1;
  clientId = -1;
}

//MonitorEntry::MonitorEntry(char * fp, int s, int d, int c){
MonitorEntry::MonitorEntry(std::string fp, time_t s, int d, int c){
  filepath = fp;
  startTime = s;
  duration = d;
  clientId = c;
}

MonitorEntry::MonitorEntry(MonitorEntry * me){

  filepath = me->filepath;
  startTime = me->startTime;
  duration = me->duration;
  clientId = me->clientId;
}

MonitorEntry::~MonitorEntry(){
  //std::cout << "Destroy" << filepath << std::endl;
}


void MonitorEntry::print(){
  std::cout << toString() << std::endl;
}

std::string MonitorEntry::toString(){
  std::string retStr;
  retStr.append("Filepath: ");
  retStr.append(filepath);
  retStr.append(" Start Time: ");
  retStr.append(std::to_string(startTime));
  retStr.append(" Duration: ");
  retStr.append(std::to_string(duration));
  retStr.append(" Client Id: ");
  retStr.append(std::to_string(clientId));
  return retStr;
}

