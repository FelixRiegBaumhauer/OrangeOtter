#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

/* These are the exceptions or our program */

typedef enum exception_code {
	FileBoundException = 0,
	NoFileException = 1,
	GeneralException = 2,
	TimeoutException = 3,
	ImproperPacketException = 4
} ExceptionCode;

struct fileBoundException : public std::exception {};
struct noFileException : public std::exception {};
struct generalException : public std::exception {};
struct timeoutException : public std::exception {};
struct improperPacketException : public std::exception {};


#endif