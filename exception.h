#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>


typedef enum exception_code {
	FileBoundException = 0,
	NoFileException = 1
} ExceptionCode;

struct fileBoundException : public std::exception {};

struct noFileException : public std::exception {};

#endif