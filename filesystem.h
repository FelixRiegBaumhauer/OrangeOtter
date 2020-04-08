#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <iostream>
#include <fstream>
#include <ctime>

#include "monitortable.h"
#include "exception.h"

#include <sys/types.h>
#include <sys/stat.h>



class FileSystem{
public:
  FileSystem();
  std::string readFile(std::string filepath, int offset, int num);
  std::vector<uint> insertFile(std::string filepath, int offset, std::string bytes);
  void monitorFile(std::string, int duration, int clientId);
  char getMode(std::string);
  std::vector<uint> shiftFile(std::string, int direction);

  //as the name implies we will be overwriting with this one
  std::vector<uint> overwriteFile(std::string filepath, std::string bytes);


  std::vector<uint> checkFile(std::string filepath);

  uint findRemove(uint clientId);

  void printMt();
  std::string readWholeFile(std::string filepath);
  time_t lastModification(std::string filepath);


private:
  std::string shiftString(std::string str, int direction);

  MonitorTable mt;

};

#endif