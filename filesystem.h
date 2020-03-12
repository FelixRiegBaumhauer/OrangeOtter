
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

//#include <stdio.h>
//#include <time.h>
//#include <string.h>



class FileSystem {

public:
  //assume that ret buf is the appropriate size, ie num+1
  int readFile(char * filepath, int offset, int num, char * ret_buf);

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
