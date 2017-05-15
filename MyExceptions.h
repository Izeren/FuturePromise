//
// Created by izeren on 5/15/17.
//
#include <stdexcept>

class ValueAlreadySet : public std::exception {
public:
    virtual const char *what() const throw();
};

class ExceptionAlreadySet : public std::exception {
public:
    virtual const char *what() const throw();
};

class FutureAlreadyGot : public std::exception {
public:
    virtual const char *what() const throw();
};

