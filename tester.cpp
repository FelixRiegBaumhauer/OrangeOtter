#include <iostream>
#include "filesystem.h"

#include <vector>
#include <string>
#include <ctime>

//#include "monitorentry.h"
#include "monitortable.h"

int main() {

  std::cout << "Testing" << std::endl;

  FileSystem fs; //= new FileSystem();

  /*
  int num = 3;
  char * buf = (char *) malloc(num+1);
  fs.readFile("testfile.txt", 0, num, buf);

  std::cout << buf << std::endl;

  fs.getMode("testfile.txt", buf);

  std::cout << buf[0] << std::endl;

  //fs.shiftFile("testfile.txt", -1);
  */

  //fs.insertFile("testfiler.txt", 0, "yes", 3);





  std::string fp1 = "A";
  std::string fp2 = "B";
  std::string fp3 = "C";
  std::string fp4 = "A";


  MonitorEntry me1 = MonitorEntry(fp1, 100, 10, 1);
  MonitorEntry me2 = MonitorEntry(fp2, 200, 20, 2);
  MonitorEntry me3 = MonitorEntry(fp3, 300, 30, 3);
  MonitorEntry me4 = MonitorEntry(fp4, 400, 40, 4);

  //MonitorEntry me5 = MonitorEntry(me4.filepath, me4.startTime, me4.duration, me4.clientId);
  MonitorEntry me5 = MonitorEntry(&me4);

  me5.startTime = 500;

  MonitorTable mt = MonitorTable();

  mt.insert(&me1);
  mt.insert(&me2);
  mt.insert(&me3);
  mt.insert(&me4);
  mt.insert(&me5);

  mt.print();

  std::vector<MonitorEntry> vec;

  mt.findAll(fp1, &vec);

  int i;
  for(i=0; i<vec.size(); i++){
    vec[i].print();
  }



  std::cout << "This Many: " << vec.size() << std::endl;




  return 0;
  
}
