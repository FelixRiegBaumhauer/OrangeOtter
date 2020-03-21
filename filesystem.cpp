


#include "filesystem.h"


FileSystem::FileSystem(){
  //mt = MonitorTable();
}


std::string FileSystem::readFile(std::string filepath, int offset, int num){
  std::string retStr, curLine;
  std::ifstream fileIn(filepath);

  //check if file exists and all
  if(!fileIn.good()){  throw noFileException();  }

  while (getline (fileIn, curLine)) {
    retStr.append(curLine);
    retStr.append("\n");
  }
  //delete the last /n
  if(retStr.size() > 0){
    retStr.pop_back();
  }

  //need to do a check
  if(offset + num > retStr.size() ){  throw fileBoundException();  }
  retStr = retStr.substr(offset, num);
  fileIn.close(); 

  return retStr;
}


void FileSystem::insertFile(std::string filepath, int offset, std::string bytes){
  std::string tempHold, curLine;
  std::ifstream fileIn(filepath);

  //check if file exists, if not we just continue with our empty string
  if(fileIn.good()){  

    while (getline (fileIn, curLine)) {
      tempHold.append(curLine);
      tempHold.append("\n");
    }
    //delete the last /n
    if(tempHold.size() > 0){
      tempHold.pop_back();
    }
  }
  fileIn.close();

  if(offset > tempHold.size() ){  throw fileBoundException();  }
  tempHold.insert(offset, bytes);
  std::ofstream fileOut(filepath);
  fileOut << tempHold;
  fileOut.close();

  //now we go ahead and monitor it
  checkFile(filepath);
}



void FileSystem::monitorFile(std::string filepath, int duration, int clientId){
  MonitorEntry me = MonitorEntry(filepath, time(0), duration, clientId);
  mt.insert(&me);
}

void FileSystem::printMt(){
  mt.print();
}

void FileSystem::checkFile(std::string filepath){
  std::vector<MonitorEntry> vec;

  //sweep is the method we use to 
  vec = mt.sweep(filepath);

  int i;
  for(i=0; i<vec.size(); i++){
    //send out each hit
    std::cout << "Send Out: " << vec[i].toString() << std::endl;
  }
}

char FileSystem::getMode(std::string filepath){
  std::string tempStr, curLine;
  std::ifstream fileIn(filepath);
  int i;
  int ic;
  int chars [256];
  char most_common;

  for(i=0; i<256; i++){
    chars[i] = 0;
  }

  //check if file exists and all
  if(!fileIn.good()){  throw noFileException();  }

  while (getline (fileIn, curLine)) {
    tempStr.append(curLine);
    tempStr.append("\n");
  }
  //delete the last /n
  if(tempStr.size() > 0){
    tempStr.pop_back();
  }
  fileIn.close(); 

  most_common = (char)0;
  for(i=0; i<tempStr.size(); i++){
    ic = (int) tempStr.at(i);
    chars[ic]++;
    if(chars[ic] > chars[(int)most_common]){
      most_common = (char) ic;
    }
  }
  
  return most_common;
}

void FileSystem::shiftFile(std::string filepath, int direction){
  std::string tempHold, curLine;
  std::ifstream fileIn(filepath);

  //check if file exists, if not we just continue with our empty string
  if(fileIn.good()){  

    while (getline (fileIn, curLine)) {
      tempHold.append(curLine);
      tempHold.append("\n");
    }
    //delete the last /n
    if(tempHold.size() > 0){
      tempHold.pop_back();
    }
  }
  fileIn.close();

  tempHold = shiftString(tempHold, direction);

  std::ofstream fileOut(filepath);
  fileOut << tempHold;
  fileOut.close();


  //now we monitor it
  checkFile(filepath);
}

std::string FileSystem::shiftString(std::string str, int direction){
  int i, ascii_val;
  std::string retStr;

  for(i=0; i<str.size(); i++){
    ascii_val = (int) str[i];
    if((ascii_val >= (int)'a' && ascii_val <= (int)'z') || (ascii_val >= (int)'A' && ascii_val <= (int)'Z')){
      ascii_val += direction; 
      if(direction > 0){
        if(ascii_val == ((int)'z')+1 ){
          ascii_val = (int)'A';
        }
        if(ascii_val == ((int)'Z')+1 ){
          ascii_val = (int)'a';
        }
      }
      else{
        if(ascii_val == ((int)'a')-1 ){
          ascii_val = (int)'Z';
        }
        if(ascii_val == ((int)'A')-1 ){
          ascii_val = (int)'z';
        }
      }
      retStr.push_back((char)ascii_val);
    }
    else{
      retStr.push_back(str[i]);
    }


  }
  return retStr;
}



/*
#include <iostream>
#include <fstream>

#include "filesystem.h"

using namespace std;


void foo(std::string s){
  return;
}

int readFile(std::string filepath, int offset, int num, std::string retStr){

  std::string curLine;
  std::ifstream fileIn(filepath);

  while (getline (fileIn, curLine)) {
    retStr.append(curLine);
    retStr.append("\n");
  }
  //delete the last /n
  retStr.pop_back();

  fileIn.close(); 

  return 0;
}



int FileSystem::readFile(char * filepath, int offset, int num, char * ret_buf){
  int fd;
  int file_size;
  struct stat stat_buf;

  if( (fd = open(filepath, O_RDONLY)) == -1){ return -1; }
  if(fstat(fd, &stat_buf) == -1){ return -1; }
  
  file_size = stat_buf.st_size;
  if( (offset + num) > file_size){ return -1; }
  
  if(read(fd, ret_buf, num) != num){ return -1; }
  ret_buf[num] = 0;

  if(close(fd) == -1){ return -1; }  
  return 0;
}

int FileSystem::insertFile(char * filepath, int offset, char * bytes, int byte_len){
  int fd;
  int file_size;
  struct stat stat_buf;

  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

  if( (fd = open(filepath, O_RDONLY | O_CREAT, mode)) == -1){ return -1; }
  if(fstat(fd, &stat_buf) == -1){ return -1; }
  
  file_size = stat_buf.st_size;
  if( offset > file_size){ return -1; }

  char * buf = (char *) malloc(file_size+byte_len);
  if(read(fd, buf, file_size) != file_size){ return -1; }
  if(close(fd) == -1){ return -1; }

  //now we do a shift
  int i=0;
  for(i=0; i<file_size-offset; i++){
    buf[file_size+byte_len-1-i] = buf[file_size-1-i];
  }

  for(i=0; i<byte_len; i++){
    buf[offset+i] = bytes[i];
  }


  if( (fd = open(filepath, O_WRONLY)) == -1){ return -1; }
  //now we overwrite back
  if(write(fd, buf, file_size+byte_len) == -1){ return -1; }

  //now we check to see if the file was on the table
  //this will handle all the updates for us
  //mt.check_entries(filename);
  
  if(close(fd) == -1){ return -1; }

  return 0;
}

int FileSystem::monitorFile(char * filepath, int duration, int client_id){
  return 0;
}

int FileSystem::getMode(char * filepath, char * ret_buf){
  int fd;
  int file_size;
  struct stat stat_buf;
  int chars [256];
  int i;
  int most_common;
    
  for(i=0; i<256; i++){ chars[i] = 0; }

  if( (fd = open(filepath, O_RDONLY)) == -1){ return -1; }
  if(fstat(fd, &stat_buf) == -1){ return -1; }
  
  file_size = stat_buf.st_size;

  //make a buffer to read in file
  char * buf = (char *) malloc(file_size);
  if(read(fd, buf, file_size) != file_size){ return -1; }
  if(close(fd) == -1){ return -1; }

  most_common = 0;
  for(i=0; i<file_size; i++){
    chars[buf[i]]++;
    if(chars[buf[i]] > chars[most_common]){
      most_common = buf[i];
    }
  }

  free(buf);
  
  ret_buf[0] = (char) most_common;
  
  return 0;
}

int FileSystem::shiftFile(char * filepath, int direction){
  int fd;
  int file_size;
  struct stat stat_buf;
  int i;

  if( (fd = open(filepath, O_RDONLY)) == -1){ return -1; }
  if(fstat(fd, &stat_buf) == -1){ return -1; }
  
  file_size = stat_buf.st_size;
  char * buf = (char *) malloc(file_size);
  if(read(fd, buf, file_size) != file_size){ return -1; }
  if(close(fd) == -1){ return -1; }

  
  //now we do a shift for all alphabetical chars
  char c;
  for(i=0; i<file_size; i++){
    c = buf[i];
      
    if(c >= 65 && c <= 90){
      c+=direction;
      if(c == 91){
	c = 65;
      }
    }

    if(c >= 97 && c <= 122){
      c+=direction;
      if(c == 123){
	c = 97;
      }
    }
    buf[i] = c;
  }

  if( (fd = open(filepath, O_WRONLY)) == -1){ return -1; }
  //now we overwrite back
  if(write(fd, buf, file_size) == -1){ return -1; }
  if(close(fd) == -1){ return -1; }

  free(buf);
  
  return 0;
}

*/