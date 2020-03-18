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
	Mode = 4
} CallType;

/*
typedef enum response_type {
	Read = 0, 
	Insert = 1, 
	Monitor = 2, 
	Shift = 3, 
	Mode = 4
} ResponseType;
*/

typedef enum success_type {
	Good = 0,
	Failure = 1,
	OtherFailure = 2
} SuccessType;

/*
typedef struct read_call{
	char * filepath;
	int offset;
	int num;
} Read_Call;

typedef struct insert_call{
	char * filepath;
	int offset;
	char * bytes;
} Insert_Call;

typedef struct monitor_call{
	char * filepath;
	int duration;
} Monitor_Call;

typedef struct shift_call{
	char * filepath;
} Shift_Call;

typedef struct mode_call{
	char * filepath;
} Mode_Call;
*/
/*
class ReadCall{
public:
	std::string filepath;
	uint offset;
	uint num;

	ReadCall(std::string filepath, uint offset, uint num);
	void print();
};

class InsertCall{
public:
	std::string filepath;
	uint offset;
	std::string bytes;

	InsertCall(std::string filepath, uint offset, std::string bytes);
	void print();
};

class MonitorCall{
public:
	std::string filepath;
	uint duration;

	MonitorCall(std::string filepath, uint duration);
	void print();
};

class ShiftCall{
public:
	std::string filepath;

	ShiftCall(std::string filepath);
	void print();
};

class ModeCall{
public:
	std::string filepath;

	ModeCall(std::string filepath);
	void print();
};
*/

class Call{
public:
	CallType callType;
	std::vector<uint> intArgs;
	std::vector<std::string> strArgs;

	Call(CallType callType, std::vector<uint> intArgs, std::vector<std::string> strArgs);
	void print();
};

class Response{
public:
	CallType respType;
	SuccessType success;
	std::string respString;

	Response(CallType respType, SuccessType success, std::string respString);
	void print();
};

class Marshal{
public:

/*
	unsigned char * marshalReadCall(ReadCall rc);
	ReadCall unmarshalReadCall(unsigned char * buf_stream);

	unsigned char * marshalInsertCall(InsertCall ic);
	InsertCall unmarshalInsertCall(unsigned char * buf_stream);

	unsigned char * marshalMonitorCall(MonitorCall mc);
	MonitorCall unmarshalMonitorCall(unsigned char * buf_stream);

	unsigned char * marshalShiftCall(ShiftCall sc);
	ShiftCall unmarshalShiftCall(unsigned char * buf_stream);

	unsigned char * marshalModeCall(ModeCall mc);
	ModeCall unmarshalModeCall(unsigned char * buf_stream);
*/

/*
	unsigned char * marshalCall(Call c);
	Call unmarshalCall(unsigned char * buf_stream);

	unsigned char * marshalResponse(Response r);
	Response unmarshalResponse(unsigned char * buf_stream);
*/

	unsigned char * marshalCall(Call c, uint * len);
	Call unmarshalCall(unsigned char * buf_stream, uint * len);

	unsigned char * marshalResponse(Response r, uint * len);
	Response unmarshalResponse(unsigned char * buf_stream, uint * len);

/*
	uint marshalCall(Call c, unsigned char ** dest);
	uint unmarshalCall(unsigned char * buf_stream, Call * dest);

	uint marshalResponse(Response r, unsigned char ** dest);
	uint unmarshalResponse(unsigned char * buf_stream, Response * dest);
*/


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