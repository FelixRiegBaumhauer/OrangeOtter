#ifndef MARSHAL_H
#define MARSHAL_H

#include <iostream>
#include <string>
#include <vector>


#define BITS_IN_BYTE 8

typedef enum call_type {
	Read = 0, 
	Insert = 1, 
	Monitor = 2, 
	Shift = 3, 
	Mode = 4,
	Ack = 5
} CallType;


typedef enum message_type {
	Call = 0,
	Response = 1,
	AckType = 2
} MessageType;


typedef enum success_type {
	Good = 0,
	Failure = 1,
	OtherFailure = 2
} SuccessType;


class Message{
public:
	MessageType type;
	CallType callType;
	std::vector<uint> intArgs;
	std::vector<std::string> strArgs;	

	Message();
	Message(MessageType type, CallType callType, std::vector<uint> intArgs, std::vector<std::string> strArgs);
	void print();
};


class Marshal{
public:
	unsigned char * marshalMessage(Message m, uint * len);
	Message unmarshalMessage(unsigned char * buf_stream, uint * len);

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