#include <iostream>
#include <fstream>

#include "filesystem.h"

using namespace std;

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

