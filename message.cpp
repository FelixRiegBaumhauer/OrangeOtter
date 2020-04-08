#include "message.h"


Message::Message(MessageType type, CallType callType, std::vector<uint> intArgs, std::vector<std::string> strArgs){
	this->type = type;
	this->callType = callType;
	this->num = 0;
	this->intArgs = intArgs;
	this->strArgs = strArgs;
}

Message::Message(MessageType type, CallType callType, uint num, std::vector<uint> intArgs, std::vector<std::string> strArgs){
	this->type = type;
	this->callType = callType;
	this->num = num;
	this->intArgs = intArgs;
	this->strArgs = strArgs;
}

Message::Message(){
	this->type = Call;
	this->callType = Read;
	this->num = 0;
	this->intArgs = {};
	this->strArgs = {};
}

void Message::setNum(uint num){
	this->num = num;
}

uint Message::getNum(){
	return num;
}

void Message::print(){
	uint i;
	std::string responseType;
	std::string messageType;

	switch(type){
		case Call: messageType = "Call";
			break;
		case Response: messageType = "Response";
			break;
	}

	switch(callType){
		case Read: responseType = "Read";
			break;
		case Insert: responseType = "Insert";
			break;
		case Monitor: responseType = "Monitor";
			break;
		case Shift: responseType = "Shift";
			break;
		case Mode: responseType = "Mode";
			break;
		case MonitorUpdate: responseType = "Monitor Update";
			break;
		case MonitorEnd: responseType = "Monitor End";
			break;
		case Dump: responseType = "Dump";
			break;
		case Fresh: responseType = "Fresh";
			break;
		default: responseType = "Default";
			break;

	}
	std::cout << "Message Type: " << messageType << " Call Type: " << responseType << " Number: " << num << " ";
	std::cout << "Int args:";
	for(i=0; i<intArgs.size(); i++){
		std::cout << " " << intArgs[i];
	}
	std::cout << " ";

	std::cout << "Str args:";
	for(i=0; i<strArgs.size(); i++){
		std::cout << " " << strArgs[i];
	}

	std::cout << std::endl;
}
