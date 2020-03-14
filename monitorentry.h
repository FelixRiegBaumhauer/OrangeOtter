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

/*
#ifndef M_ENTRY_H
#define M_ENTRY_H

#include <string>
#include <ctime>

class MonitorEntry{
public:
  //char * filepath;
  std::string filepath;
  time_t startTime;
  int duration;
  int clientId;  
  MonitorEntry * next;

  MonitorEntry();
  //MonitorEntry(char * fp, int s, int d, int c);
  MonitorEntry(std::string fp, time_t s, int d, int c);
  MonitorEntry(MonitorEntry * me);
  ~MonitorEntry();
  void print();
  std::string toString();

};

#endif
*/

#ifndef M_ENTRY_H
#define M_ENTRY_H

#include <string>
#include <ctime>
#include <iostream>

class MonitorEntry{
public:
  std::string filepath;
  time_t startTime;
  int duration;
  int clientId;

  MonitorEntry();
  MonitorEntry(std::string fp, time_t s, int d, int c);
  MonitorEntry(MonitorEntry * me);
  ~MonitorEntry();
  void print();
  std::string toString();

};

#endif