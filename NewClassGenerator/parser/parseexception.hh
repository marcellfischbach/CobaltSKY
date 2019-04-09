
#pragma once

#include <exception>
#include <string>

namespace cs::classgenerator
{

class ParseException : public std::exception
{
public:
  ParseException(const std::string& message = "")
    : std::exception (message.c_str())
  {

  }
};
}