#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

struct fileBoundException : public std::exception {
};

struct noFileException : public std::exception {
};

#endif