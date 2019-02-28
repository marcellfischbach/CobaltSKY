
#pragma once

#include <exception>
#include <string>

namespace cs::editor::model
{


class IllegalTypeException : public std::exception 
{
public:
  IllegalTypeException(const std::string &message)
    : std::exception(message.c_str())
  {

  }
};


}