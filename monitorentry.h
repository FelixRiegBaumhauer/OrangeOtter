/*
#include <time.h>

class MonitorEntry{
public:
  char * filepath;
  int start_time;
  int duration;
  int client_id;
  MonitorEntry * next;

  MonitorEntry();
  //~MonitorEntry();  
  MonitorEntry(char * fp, int st, int d, int cid);
  void print();
};
*/

#ifndef M_ENTRY_H
#define M_ENTRY_H

#include <string>

class MonitorEntry{
public:
  //char * filepath;
  std::string filepath;
  int startTime;
  int duration;
  int clientId;  
  MonitorEntry * next;

  MonitorEntry();
  //MonitorEntry(char * fp, int s, int d, int c);
  MonitorEntry(std::string fp, int s, int d, int c);
  MonitorEntry(MonitorEntry * me);
  ~MonitorEntry();
  void print();

};

#endif