#include "marshal.h"


void Marshal::intToChar(uint src, unsigned char * dest){
	uint i, nsrc;
	uint uint_size = sizeof(uint);
	uint char_size = sizeof(unsigned char);

	nsrc = htonl(src); //not technically needed
	for(i=0; i<uint_size; i++){
		dest[i] = nsrc >> (uint_size - i - 1)*BITS_IN_BYTE;
	}
}

int Marshal::charToInt(unsigned char * src){
	uint i, result, hresult;

	result = 0;
	uint uint_size = sizeof(uint);
	uint char_size = sizeof(unsigned char);
	for(i=0; i<uint_size; i++){
		result = result << (BITS_IN_BYTE);
		result |= src[i];
	}
	hresult = ntohl(result); //also not needed

	return hresult;
}

uint Marshal::packageString(std::string src, unsigned char * dest){
	uint bytes_writen = 0;
	uint str_len;
	uint uint_size = sizeof(uint);
	uint i;
	uint dif;

	str_len = src.length();
	intToChar(str_len, dest); /* We first append the length */
	dest = dest + uint_size;
	bytes_writen += uint_size;

	//copy over the string
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

uint Marshal::stringsLength(std::vector<std::string> strs){
	uint uint_size = sizeof(uint);
	uint len = 0;
	uint size = strs.size();
	uint dif = 0;
	uint i;

	len += uint_size;
	
	for(i=0; i<size; i++){
		len += stringLength(strs[i]);
	}
	return len;
}

uint Marshal::intsLength(std::vector<uint> ints){
	uint uint_size = sizeof(uint);
	uint len = 0;
	uint size = ints.size();
	uint dif = 0;
	uint i;

	len += uint_size;
	for(i=0; i<size; i++){
		len += uint_size;
	}
	return len;
}

uint Marshal::packageStrings(std::vector<std::string> src, unsigned char * dest){
	//first calculate the num of strings
	uint bytes_writen = 0;
	uint uint_size = sizeof(uint);
	uint num_strs;
	uint i;

	num_strs = src.size();

	intToChar(num_strs, dest);
	dest = dest + uint_size;
	bytes_writen += uint_size;

	for(i=0; i<num_strs; i++){
		uint added_bytes = 0;
		added_bytes = packageString(src[i], dest);
		dest += added_bytes;
		bytes_writen += added_bytes;
	}

	return bytes_writen;
}

uint Marshal::unpackageStrings(unsigned char * src, std::vector<std::string> * dest){
	uint bytes_writen = 0;
	uint num_strs;
	uint uint_size = sizeof(uint);
	uint i;

	dest->clear();

	num_strs = charToInt(src);
	src += uint_size;
	bytes_writen += uint_size;

	for(i=0; i<num_strs; i++){
		std::string tempStr;
		uint added_bytes = 0;
		added_bytes = unpackageString(src, &tempStr );
		src += added_bytes;
		bytes_writen += added_bytes;
		dest->push_back(tempStr);
	}

	return bytes_writen;
}

uint Marshal::packageInts(std::vector<uint> src, unsigned char * dest){
	//first calculate the num of strings
	uint bytes_writen = 0;
	uint uint_size = sizeof(uint);
	uint num_ints;
	uint i;

	num_ints = src.size();

	intToChar(num_ints, dest);
	dest = dest + uint_size;
	bytes_writen += uint_size;

	for(i=0; i<num_ints; i++){
		intToChar(src[i], dest);
		dest = dest + uint_size;
		bytes_writen += uint_size;
	}

	return bytes_writen;
}

uint Marshal::unpackageInts(unsigned char * src, std::vector<uint> * dest){
	uint bytes_writen = 0;
	uint num_ints;
	uint uint_size = sizeof(uint);
	uint i;

	dest->clear();

	num_ints = charToInt(src);
	src += uint_size;
	bytes_writen += uint_size;

	for(i=0; i<num_ints; i++){
		uint tempInt;
		tempInt = charToInt(src);
		src += uint_size;
		bytes_writen += uint_size;
		dest->push_back(tempInt);
	}

	return bytes_writen;
}

uint Marshal::getMarshalLength(unsigned char * buf){
	return charToInt(buf);
}

unsigned char * Marshal::marshalMessage(Message m, uint * len){
	uint uint_size = sizeof(uint);
	uint size;
	unsigned char * buf_stream;
	uint pos = 0;
	uint str_space;
	uint i;

	size = 4*uint_size;
	size += intsLength(m.intArgs);
	size += stringsLength(m.strArgs);

	buf_stream = (unsigned char *) malloc(size);

	intToChar(size, buf_stream+pos);
	pos += uint_size;
	intToChar(m.type, buf_stream+pos);
	pos += uint_size;
	intToChar(m.callType, buf_stream+pos);
	pos += uint_size;
	intToChar(m.num, buf_stream+pos);
	pos += uint_size;

	str_space = packageInts(m.intArgs, buf_stream+pos);
	pos += str_space;


	str_space = packageStrings(m.strArgs, buf_stream+pos);
	pos += str_space;

	*len = pos;

	return buf_stream;
}

Message Marshal::unmarshalMessage(unsigned char * buf_stream, uint * len){
	uint uint_size = sizeof(uint);
	uint size;
	uint pos = 0;
	uint str_space;

	MessageType messageType;
	CallType callType;
	std::vector<uint> intArgs;
	std::vector<std::string> strArgs;
	uint num;

	//no real need for this
	size = charToInt(buf_stream+pos);
	pos += uint_size;
	messageType = (MessageType)charToInt(buf_stream+pos);
	pos += uint_size;
	callType = (CallType)charToInt(buf_stream+pos);
	pos += uint_size;
	num = (uint)charToInt(buf_stream+pos);
	pos += uint_size;

	str_space = unpackageInts(buf_stream+pos, &intArgs);
	pos += str_space;

	str_space = unpackageStrings(buf_stream+pos, &strArgs);
	pos += str_space;

	Message m = Message(messageType, callType, num, intArgs, strArgs);

	*len = pos;

	return m;
}