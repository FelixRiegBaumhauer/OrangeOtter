#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <vector>

typedef enum call_type {
	Read = 0, 
	Insert = 1, 
	Monitor = 2, 
	Shift = 3, 
	Mode = 4,
	MonitorUpdate = 5,
	MonitorEnd = 6,
	Fresh = 7,
	Dump = 8
} CallType;


typedef enum message_type {
	Call = 0,
	Response = 1
} MessageType;


class Message{
public:
	MessageType type;
	CallType callType;
	uint num;
	std::vector<uint> intArgs;
	std::vector<std::string> strArgs;	

	Message();
	Message(MessageType type, CallType callType, uint num, std::vector<uint> intArgs, std::vector<std::string> strArgs);
	Message(MessageType type, CallType callType, std::vector<uint> intArgs, std::vector<std::string> strArgs);
	void setNum(uint num);
	uint getNum();
	void print();
};

#endif