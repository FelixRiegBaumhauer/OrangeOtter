#ifndef MARSHAL_H
#define MARSHAL_H

#include <iostream>
#include <string>
#include <vector>
#include <arpa/inet.h>

#include "message.h"

#define BITS_IN_BYTE 8

class Marshal{

	/* The Marshal takes messages and converts them into byte streams */

public:

	/* For all functions that return uint, these are the total number of bytes written out or read in */

	/* Convert a message into a stream of bytes */
	unsigned char * marshalMessage(Message m, uint * len);
	Message unmarshalMessage(unsigned char * buf_stream, uint * len);

	/* Get teh length of the byte stream */
	uint getMarshalLength(unsigned char * buf);

	/* Package list of strings string into byte stream */
	uint packageStrings(std::vector<std::string> src, unsigned char * dest);
	uint unpackageStrings(unsigned char * src, std::vector<std::string> * dest);

	/* Package list of ints into byte stream */
	uint packageInts(std::vector<uint> src, unsigned char * dest);
	uint unpackageInts(unsigned char * src, std::vector<uint> * dest);

	/* Convert int to byte stream */
	void intToChar(uint src, unsigned char * dest);
	int charToInt(unsigned char * src);

	//package String into byte stream */
	uint packageString(std::string src, unsigned char * dest);
	uint unpackageString(unsigned char * src, std::string * dest);

	/* Get lengths from byte streams */
	uint stringLength(std::string s);
	uint stringsLength(std::vector<std::string> strs);
	uint intsLength(std::vector<uint> ints);

};

#endif