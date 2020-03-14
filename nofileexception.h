#ifndef NO_FILE_EXCEPTION
#define NO_FILE_EXCEPTION

#include <exception>

struct noFileException : public std::exception {
};

#endif