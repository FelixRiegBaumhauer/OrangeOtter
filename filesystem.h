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

/* FileSystem is the underlying system that interacts with files */

public:
  FileSystem();

  /* Reads the file*/
  std::string readFile(std::string filepath, int offset, int num);
  
  /* Insert into file */
  std::vector<uint> insertFile(std::string filepath, int offset, std::string bytes);
  
  /* Monitor file */
  void monitorFile(std::string, int duration, int clientId);

  /* Get thee mode, ie the most common character, otherwise returns a random character */
  char getMode(std::string);
  
  /* Shift the file, we only allow positive shifts */
  std::vector<uint> shiftFile(std::string, int direction);

  /* Overwrite the file with the bytes */
  std::vector<uint> overwriteFile(std::string filepath, std::string bytes);

  /* Check if there are any pending monitors on the given filepath, if we find matches, we perpare a list of clientIds */
  std::vector<uint> checkFile(std::string filepath);

  /* Remove an entry from the monitor table, which we do after we finish a monitorinig of a file */
  uint findRemove(uint clientId);

  /* Dump the whole file in one go */
  std::string readWholeFile(std::string filepath);
  
  /* Get the last modification time */
  time_t lastModification(std::string filepath);

  // for debuging
  void printMt();

private:
  /* Used to shift strings */
  std::string shiftString(std::string str, int direction);

  MonitorTable mt;

};

#endif