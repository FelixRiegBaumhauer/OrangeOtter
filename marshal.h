#ifndef MARSHAL_H
#define MARSHAL_H

#include <iostream>
#include <string>
#include <vector>
#include <arpa/inet.h>

#include "message.h"

#define BITS_IN_BYTE 8

class Marshal{
public:
	unsigned char * marshalMessage(Message m, uint * len);
	Message unmarshalMessage(unsigned char * buf_stream, uint * len);

	uint getMarshalLength(unsigned char * buf);

	uint packageStrings(std::vector<std::string> src, unsigned char * dest);
	uint unpackageStrings(unsigned char * src, std::vector<std::string> * dest);


	uint packageInts(std::vector<uint> src, unsigned char * dest);
	uint unpackageInts(unsigned char * src, std::vector<uint> * dest);


	void intToChar(uint src, unsigned char * dest);
	int charToInt(unsigned char * src);

	//returns the number of bytes written out
	uint packageString(std::string src, unsigned char * dest);
	uint unpackageString(unsigned char * src, std::string * dest);

	uint stringLength(std::string s);
	uint stringsLength(std::vector<std::string> strs);
	uint intsLength(std::vector<uint> ints);

};

#endif