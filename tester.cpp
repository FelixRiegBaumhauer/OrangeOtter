#include <iostream>
#include "filesystem.h"

#include <vector>
#include <string>
#include <ctime>
#include <fstream>

#include <unistd.h>

//#include "monitorentry.h"
#include "monitortable.h"
#include "nofileexception.h"
#include "fileboundexception.h"

int main() {

  std::cout << "Testing" << std::endl;
  FileSystem fs;

  std::string inputText;

  while(1){

    std::cin >> inputText;
    std::cout << "We wrote: " << inputText << std::endl;

    if(inputText.compare("read") == 0){
      std::string filepath;
      std::string offset;
      std::string num;
      std::string resStr;

      std::cout << "Input Filepath" << std::endl;
      std::cin >> filepath;
      std::cout << "Input the offset position" << std::endl;
      std::cin >> offset;
      std::cout << "Input Number of Characters" << std::endl;
      std::cin >> num;

      resStr = fs.readFile(filepath, std::stoi(offset), std::stoi(num));
      std::cout << resStr << std::endl;
    }

    if(inputText.compare("insert") == 0){
      std::string filepath;
      std::string offset;
      std::string bytes;

      std::cout << "Input Filepath" << std::endl;
      std::cin >> filepath;
      std::cout << "Input the offset position" << std::endl;
      std::cin >> offset;
      std::cout << "Input Text" << std::endl;
      std::cin >> bytes;

      fs.insertFile(filepath, std::stoi(offset), bytes);
    }

    if(inputText.compare("monitor") == 0){
      std::string filepath;
      std::string duration;
      std::string clientId;

      std::cout << "Input Filepath" << std::endl;
      std::cin >> filepath;
      std::cout << "Input the duration" << std::endl;
      std::cin >> duration;
      std::cout << "Input clientId" << std::endl;
      std::cin >> clientId;

      fs.monitorFile(filepath, std::stoi(duration), std::stoi(clientId));
    }

    if(inputText.compare("mode") == 0){
      std::string filepath;
      char c;

      std::cout << "Input Filepath" << std::endl;
      std::cin >> filepath;

      c = fs.getMode(filepath);
      std::cout << "The Most common letter is: " << c << std::endl;
    }

    if(inputText.compare("shift") == 0){
      std::string filepath;
      std::string direction;

      std::cout << "Input Filepath" << std::endl;
      std::cin >> filepath;
      std::cout << "Input the direction" << std::endl;
      std::cin >> direction;

      fs.shiftFile(filepath, std::stoi(direction));
    }



    if(inputText.compare("q") == 0){
      std::cout << "Exiting Program" << std::endl;
      return 0;
    }
  }



  //FileSystem fs; //= new FileSystem();

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


  /*
  std::string file;
  std::string filename = "testfile.txt";

  char c = fs.getMode(filename);

  std::cout << c << std::endl;

  fs.monitorFile(filename, 100, 1);
  fs.monitorFile(filename, 3, 2);

  fs.insertFile(filename, 0, "hello");

  std::cout << "Before Sleep" << std::endl;
  sleep(5);
  std::cout << "After Sleep" << std::endl;

  fs.insertFile(filename, 0, "not");
  */


  //we would do a checkFile after every insert or shift


  //fs.shiftFile("abc.txt", 1);

/*
  try{
    fs.readFile(filename, 0, 100, &file);
  }
  catch(noFileException e){
    std::cout << "No File" << std::endl;
  }
  catch(fileBoundException e){
    std::cout << "File Bounds Off" << std::endl;
  }
  */




/*
  try{
    throw noFileException();
  }
  catch(noFileException e){
    std::cout << "NO File" << std::endl;
  }
  */



/*
// Create a text string, which is used to output the text file
std::string myText;
std::string fullText;

// Read from the text file
std::ifstream MyReadFile("testfile.txt");

// Use a while loop together with the getline() function to read the file line by line

while (getline (MyReadFile, myText)) {
  // Output the text from the file
  //std::cout << myText << std::endl;
  fullText.append(myText);
  fullText.append("\n");
}
fullText.pop_back();

std::cout <<"Start" << fullText << "End" << std::endl;

// Close the file
MyReadFile.close(); 
*/



/*
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
*/



  return 0;
  
}
