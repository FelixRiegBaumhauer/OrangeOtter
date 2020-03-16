


#include "marshal.h"




void Marshal::intToChar(uint src, unsigned char * dest){
	uint i;
	uint uint_size = sizeof(uint);
	uint char_size = sizeof(unsigned char);
	for(i=0; i<uint_size; i++){
		dest[i] = src >> (uint_size - i - 1)*BITS_IN_BYTE;
	}
}
int Marshal::charToInt(unsigned char * src){
	uint i, result;

	result = 0;

	uint uint_size = sizeof(uint);
	uint char_size = sizeof(unsigned char);
	for(i=0; i<uint_size; i++){

		result = result << (BITS_IN_BYTE);
		result |= src[i];
	}

	return result;
}

uint Marshal::packageString(std::string src, unsigned char * dest){
	uint bytes_writen = 0;
	uint str_len;
	uint uint_size = sizeof(uint);
	uint i;
	uint dif;

	str_len = src.length();

	intToChar(str_len, dest);
	dest = dest + uint_size;
	bytes_writen += uint_size;

	//copy over
	for(i=0; i<str_len; i++){
		dest[i] = src.at(i);
	}

	dest = dest + str_len;
	bytes_writen += str_len;

	//buffer out to word boundary
	dif = uint_size - (str_len % uint_size);
	if(dif == 4){ dif = 0; }

	for(i=0; i<dif; i++){
		dest[i] = (unsigned char)0;
	}

	bytes_writen += dif;

	return bytes_writen;
}
/*
std::string Marshal::unpackageString(unsigned char * src){
	std::string dest;

	uint str_len;
	uint uint_size = sizeof(uint);
	uint i;

	str_len = charToInt(src);
	src+= uint_size;

	for(i=0; i<str_len; i++){
		dest.push_back(src[i]);
	}

	return dest;
}
*/

uint Marshal::unpackageString(unsigned char * src, std::string * dest){
	uint bytes_writen = 0;
	uint str_len;
	uint uint_size = sizeof(uint);
	uint i;
	uint dif;

	dest->clear();

	str_len = charToInt(src);
	src+= uint_size;
	bytes_writen += uint_size;

	for(i=0; i<str_len; i++){
		dest->push_back(src[i]);
	}
	bytes_writen += str_len;

	//measure out the buffer
	dif = uint_size - (str_len % uint_size);
	if(dif == 4){ dif = 0; }

	bytes_writen += dif;


	return bytes_writen;
}

uint Marshal::stringLength(std::string s){
	uint uint_size = sizeof(uint);
	uint len = 0;
	uint size = s.size();
	uint dif = 0;

	dif = uint_size - (size % uint_size);
	if(dif == 4){ dif = 0; }

	len += uint_size;
	len += (size + dif);

	return len;
}

unsigned char * Marshal::marshalReadCall(ReadCall rc){
	uint uint_size = sizeof(uint);
	uint size;
	unsigned char * buf_stream;
	uint pos = 0;
	uint str_space;


	//first we calculate the total size of our byte stream
	//size type [int args] [str args]
	//for a read call we have 2 int args and 1 string arg
	size = 4*uint_size + stringLength(rc.filepath);

	buf_stream = (unsigned char *) malloc(size);

	intToChar(size, buf_stream+pos);
	pos += uint_size;
	intToChar(Read, buf_stream+pos);
	pos += uint_size;
	intToChar(rc.offset, buf_stream+pos);
	pos += uint_size;
	intToChar(rc.num, buf_stream+pos);
	pos += uint_size;

	str_space = packageString(rc.filepath, buf_stream+pos);
	pos += str_space;

	return buf_stream;
}

ReadCall Marshal::unmarshallReadCall(unsigned char * buf_stream){
	uint uint_size = sizeof(uint);
	uint size;
	uint offset;
	uint num;
	std::string filepath;

	uint pos = 0;
	uint str_space;

	//no real for this
	size = charToInt(buf_stream+pos);
	pos += uint_size;

	//skip over the type
	pos += uint_size;

	offset = charToInt(buf_stream+pos);
	pos += uint_size;
	num = charToInt(buf_stream+pos);
	pos += uint_size;

	str_space = unpackageString(buf_stream+pos, &filepath);
	pos += str_space;

	ReadCall rc = ReadCall(filepath, offset, num);

	return rc;
}

unsigned char * Marshal::marshalInsertCall(InsertCall ic){
	uint uint_size = sizeof(uint);
	uint size;
	unsigned char * buf_stream;
	uint pos = 0;
	uint str_space;


	//first we calculate the total size of our byte stream
	//size type [int args] [str args]
	//for a read call we have 2 int args and 1 string arg
	size = 3*uint_size + stringLength(ic.filepath) + stringLength(ic.bytes);

	buf_stream = (unsigned char *) malloc(size);

	intToChar(size, buf_stream+pos);
	pos += uint_size;
	intToChar(Insert, buf_stream+pos);
	pos += uint_size;
	intToChar(ic.offset, buf_stream+pos);
	pos += uint_size;

	str_space = packageString(ic.filepath, buf_stream+pos);
	pos += str_space;

	str_space = packageString(ic.bytes, buf_stream+pos);
	pos += str_space;

	return buf_stream;
}
InsertCall Marshal::unmarshallInsertCall(unsigned char * buf_stream){
	uint uint_size = sizeof(uint);
	uint size;
	uint offset;
	std::string filepath;
	std::string bytes;

	uint pos = 0;
	uint str_space;

	//no real need for this
	size = charToInt(buf_stream+pos);
	pos += uint_size;

	//skip over the type
	pos += uint_size;

	offset = charToInt(buf_stream+pos);
	pos += uint_size;

	str_space = unpackageString(buf_stream+pos, &filepath);
	pos += str_space;

	str_space = unpackageString(buf_stream+pos, &bytes);
	pos += str_space;

	InsertCall ic = InsertCall(filepath, offset, bytes);

	return ic;
}


unsigned char * Marshal::marshalMonitorCall(MonitorCall mc){
	uint uint_size = sizeof(uint);
	uint size;
	unsigned char * buf_stream;
	uint pos = 0;
	uint str_space;


	//first we calculate the total size of our byte stream
	//size type [int args] [str args]
	//for a read call we have 2 int args and 1 string arg
	size = 3*uint_size + stringLength(mc.filepath);

	buf_stream = (unsigned char *) malloc(size);

	intToChar(size, buf_stream+pos);
	pos += uint_size;
	intToChar(Monitor, buf_stream+pos);
	pos += uint_size;
	intToChar(mc.duration, buf_stream+pos);
	pos += uint_size;

	str_space = packageString(mc.filepath, buf_stream+pos);
	pos += str_space;

	return buf_stream;
}
MonitorCall Marshal::unmarshallMonitorCall(unsigned char * buf_stream){
	uint uint_size = sizeof(uint);
	uint size;
	uint duration;
	std::string filepath;

	uint pos = 0;
	uint str_space;

	//no real need for this
	size = charToInt(buf_stream+pos);
	pos += uint_size;

	//skip over the type
	pos += uint_size;

	duration = charToInt(buf_stream+pos);
	pos += uint_size;

	str_space = unpackageString(buf_stream+pos, &filepath);
	pos += str_space;

	MonitorCall mc = MonitorCall(filepath, duration);

	return mc;
}

unsigned char * Marshal::marshalShiftCall(ShiftCall sc){
	uint uint_size = sizeof(uint);
	uint size;
	unsigned char * buf_stream;
	uint pos = 0;
	uint str_space;


	//first we calculate the total size of our byte stream
	//size type [int args] [str args]
	//for a read call we have 2 int args and 1 string arg
	size = 2*uint_size + stringLength(sc.filepath);

	buf_stream = (unsigned char *) malloc(size);

	intToChar(size, buf_stream+pos);
	pos += uint_size;
	intToChar(Shift, buf_stream+pos);
	pos += uint_size;

	str_space = packageString(sc.filepath, buf_stream+pos);
	pos += str_space;

	return buf_stream;
}
ShiftCall Marshal::unmarshallShiftCall(unsigned char * buf_stream){
	uint uint_size = sizeof(uint);
	uint size;
	std::string filepath;

	uint pos = 0;
	uint str_space;

	//no real need for this
	size = charToInt(buf_stream+pos);
	pos += uint_size;

	//skip over the type
	pos += uint_size;

	str_space = unpackageString(buf_stream+pos, &filepath);
	pos += str_space;

	ShiftCall sc = ShiftCall(filepath);

	return sc;
}

unsigned char * Marshal::marshalModeCall(ModeCall mc){
	uint uint_size = sizeof(uint);
	uint size;
	unsigned char * buf_stream;
	uint pos = 0;
	uint str_space;


	//first we calculate the total size of our byte stream
	//size type [int args] [str args]
	//for a read call we have 2 int args and 1 string arg
	size = 2*uint_size + stringLength(mc.filepath);

	buf_stream = (unsigned char *) malloc(size);

	intToChar(size, buf_stream+pos);
	pos += uint_size;
	intToChar(Mode, buf_stream+pos);
	pos += uint_size;

	str_space = packageString(mc.filepath, buf_stream+pos);
	pos += str_space;

	return buf_stream;
}
ModeCall Marshal::unmarshallModeCall(unsigned char * buf_stream){
	uint uint_size = sizeof(uint);
	uint size;
	std::string filepath;

	uint pos = 0;
	uint str_space;

	//no real need for this
	size = charToInt(buf_stream+pos);
	pos += uint_size;

	//skip over the type
	pos += uint_size;

	str_space = unpackageString(buf_stream+pos, &filepath);
	pos += str_space;

	ModeCall mc = ModeCall(filepath);

	return mc;
}

unsigned char * Marshal::marshalResponse(Response r){
	uint uint_size = sizeof(uint);
	uint size;
	unsigned char * buf_stream;
	uint pos = 0;
	uint str_space;


	//first we calculate the total size of our byte stream
	//size type [int args] [str args]
	//for a read call we have 2 int args and 1 string arg
	size = 3*uint_size + stringLength(r.respString);

	buf_stream = (unsigned char *) malloc(size);

	intToChar(size, buf_stream+pos);
	pos += uint_size;
	intToChar(r.respType, buf_stream+pos);
	pos += uint_size;
	intToChar(r.success, buf_stream+pos);
	pos += uint_size;

	str_space = packageString(r.respString, buf_stream+pos);
	pos += str_space;

	return buf_stream;
}
Response Marshal::unmarshalResponse(unsigned char * buf_stream){
	uint uint_size = sizeof(uint);
	uint size;

	CallType respType;
	SuccessType success;
	std::string respString;

	uint pos = 0;
	uint str_space;

	//no real need for this
	size = charToInt(buf_stream+pos);
	pos += uint_size;
	respType = (CallType)charToInt(buf_stream+pos);
	pos += uint_size;
	success = (SuccessType)charToInt(buf_stream+pos);
	pos += uint_size;

	str_space = unpackageString(buf_stream+pos, &respString);
	pos += str_space;

	Response r = Response(respType, success, respString);

	return r;
}

int main(){

	Marshal marshal;

	std::cout << "Main" << std::endl;

	std::string filepath = "abc.txt";
	std::string bytes = "hello";
	unsigned char * buf;


	ReadCall rc1 = ReadCall(filepath, 0, 10);
	InsertCall ic1 = InsertCall(filepath, 0, bytes);
	MonitorCall mc1 = MonitorCall(filepath, 100);
	ShiftCall sc1 = ShiftCall(filepath);
	ModeCall moc1 = ModeCall(filepath);

	rc1.print();
	buf = marshal.marshalReadCall(rc1);
	ReadCall rc2 = marshal.unmarshallReadCall(buf);
	rc2.print();
	free(buf);

	ic1.print();
	buf = marshal.marshalInsertCall(ic1);
	InsertCall ic2 = marshal.unmarshallInsertCall(buf);
	ic2.print();
	free(buf);

	mc1.print();
	buf = marshal.marshalMonitorCall(mc1);
	MonitorCall mc2 = marshal.unmarshallMonitorCall(buf);
	mc2.print();
	free(buf);

	sc1.print();
	buf = marshal.marshalShiftCall(sc1);
	ShiftCall sc2 = marshal.unmarshallShiftCall(buf);
	sc2.print();
	free(buf);

	moc1.print();
	buf = marshal.marshalModeCall(moc1);
	ModeCall moc2 = marshal.unmarshallModeCall(buf);
	moc2.print();
	free(buf);



	Response r1 = Response(Read, Good, "Hello World");
	Response r2 = Response(Read, Failure, "");
	Response r3 = Response(Read, OtherFailure, "");
	Response r4 = Response(Insert, Good, "");
	Response r5 = Response(Monitor, Good, "");
	Response r6 = Response(Shift, Good, "");
	Response r7 = Response(Mode, Good, "");


	r1.print();
	buf = marshal.marshalResponse(r1);
	Response r10 = marshal.unmarshalResponse(buf);
	r10.print();
	free(buf);

	r2.print();
	buf = marshal.marshalResponse(r2);
	Response r20 = marshal.unmarshalResponse(buf);
	r20.print();
	free(buf);

	r3.print();
	buf = marshal.marshalResponse(r3);
	Response r30 = marshal.unmarshalResponse(buf);
	r30.print();
	free(buf);

	r4.print();
	buf = marshal.marshalResponse(r4);
	Response r40 = marshal.unmarshalResponse(buf);
	r40.print();
	free(buf);

	r5.print();
	buf = marshal.marshalResponse(r5);
	Response r50 = marshal.unmarshalResponse(buf);
	r50.print();
	free(buf);

	r6.print();
	buf = marshal.marshalResponse(r6);
	Response r60 = marshal.unmarshalResponse(buf);
	r60.print();
	free(buf);

	r7.print();
	buf = marshal.marshalResponse(r7);
	Response r70 = marshal.unmarshalResponse(buf);
	r70.print();
	free(buf);




}






ReadCall::ReadCall(std::string filepath, uint offset, uint num){
	this->filepath = filepath;
	this->offset = offset;
	this->num = num;
}
void ReadCall::print(){
	std::cout << "Read Call: fp: " << filepath << " offset: " << offset << " num: " << num << std::endl;
}


InsertCall::InsertCall(std::string filepath, uint offset, std::string bytes){
	this->filepath = filepath;
	this->offset = offset;
	this->bytes = bytes;
}
void InsertCall::print(){
	std::cout << "Insert Call: fp: " << filepath << " offset: " << offset << " bytes: "<< bytes << std::endl;
}

MonitorCall::MonitorCall(std::string filepath, uint duration){
	this->filepath = filepath;
	this->duration = duration;
}
void MonitorCall::print(){
	std::cout << "Monitor Call: fp: " << filepath << " duration: " << duration << std::endl;
}

ShiftCall::ShiftCall(std::string filepath){
	this->filepath = filepath;
}
void ShiftCall::print(){
	std::cout << "Shift Call: fp: " << filepath << std::endl;
}

ModeCall::ModeCall(std::string filepath){
	this->filepath = filepath;
}
void ModeCall::print(){
	std::cout << "Mode Call: fp: " << filepath << std::endl;
}


Response::Response(CallType respType, SuccessType success, std::string respString){
	this->respType = respType;
	this->success = success;
	this->respString = respString;
}
void Response::print(){

	std::string responseType;

	switch(respType){
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
	}


	std::cout << "Response: Response Type: " << responseType << " Success Type: " << success << " Response String: " << respString << std::endl;
}