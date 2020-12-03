// Copyright 2017 UBC Sailbot

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <string>
#include <stdexcept>

namespace NetworkTable {

class NotConnectedException : public std::runtime_error {
public:
     explicit NotConnectedException(const std::string &what) : std::runtime_error(what.c_str()) { };
};

class NodeNotFoundException : public std::runtime_error {
public:
    explicit NodeNotFoundException(const std::string &what) : std::runtime_error(what.c_str()) { };
};

class InterruptedException : public std::runtime_error {
public:
    explicit InterruptedException(const std::string &what) : std::runtime_error(what.c_str()) { };
};

}  // namespace NetworkTable

#endif  // EXCEPTIONS_H_
