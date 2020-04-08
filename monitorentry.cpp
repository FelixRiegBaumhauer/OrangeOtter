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

