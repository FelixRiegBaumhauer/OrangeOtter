#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <iostream>
#include <fstream>
#include <ctime>

#include "monitortable.h"
#include "nofileexception.h"
#include "fileboundexception.h"

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

  std::vector<uint> checkFile(std::string filepath);

  uint findRemove(uint clientId);

  void printMt();
  std::string readWholeFile(std::string filepath);
  time_t lastModification(std::string filepath);


private:
  std::string shiftString(std::string str, int direction);

  MonitorTable mt;


};



/*
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

//#include <stdio.h>
//#include <time.h>
//#include <string.h>


#include <string>

class FileSystem {

public:

  void foo(std::string s);

  //assume that ret buf is the appropriate size, ie num+1
  int readFile(char * filepath, int offset, int num, char * ret_buf);

  int readFile(std::string filepath, int offset, int num, std::string retStr);

  //0 is ok, -1 for error
  int insertFile(char * filepath, int offset, char * bytes, int byte_len);

  //0 is ok, -1 for error
  int monitorFile(char * filepath, int duration, int client_id);

  //0 is ok, -1 is error
  int getMode(char * filepath, char * ret_buf);

  //0 is ok, -1 is error
  //1 for up, -1 for down
  int shiftFile(char * filepath, int direction);
};
*/


#endif