
#include "filesystem.h"

FileSystem::FileSystem(){
}

time_t FileSystem::lastModification(std::string filepath){
  struct stat buf;
  const char *c;

  c = filepath.c_str();
  stat(c, &buf);
  return buf.st_mtim.tv_sec;
}

std::string FileSystem::readWholeFile(std::string filepath){
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

  fileIn.close(); 

  return retStr;
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


std::vector<uint> FileSystem::insertFile(std::string filepath, int offset, std::string bytes){
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
  return checkFile(filepath);
}



void FileSystem::monitorFile(std::string filepath, int duration, int clientId){
  MonitorEntry me = MonitorEntry(filepath, time(0), duration, clientId);
  mt.insert(&me);
}

void FileSystem::printMt(){
  mt.print();
}

std::vector<uint> FileSystem::checkFile(std::string filepath){
  std::vector<MonitorEntry> vec;
  std::vector<uint> clientInts;

  //sweep is the method we use to find hits
  vec = mt.sweep(filepath);

  int i;
  for(i=0; i<vec.size(); i++){
    //send out each hit
    std::cout << "Send Out: " << vec[i].toString() << std::endl;
    clientInts.push_back(vec[i].clientId);
  }
  return clientInts;
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

std::vector<uint> FileSystem::shiftFile(std::string filepath, int direction){
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
  return checkFile(filepath);
}

std::vector<uint> FileSystem::overwriteFile(std::string filepath, std::string bytes){
    std::ofstream fileOut(filepath);
    fileOut << bytes;
    fileOut.close();
    return checkFile(filepath);
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

uint FileSystem::findRemove(uint clientId){
  return mt.findRemove(clientId);
}
