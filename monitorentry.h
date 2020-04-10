#ifndef M_ENTRY_H
#define M_ENTRY_H

#include <string>
#include <ctime>
#include <iostream>

class MonitorEntry{

  /* Monitor Entry is used to note if we need to alert a client of chang in a file */

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