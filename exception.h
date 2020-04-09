#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

typedef enum exception_code {
	FileBoundException = 0,
	NoFileException = 1,
	GeneralException = 2
} ExceptionCode;

struct fileBoundException : public std::exception {};
struct noFileException : public std::exception {};
struct generalException : public std::exception {};

#endif