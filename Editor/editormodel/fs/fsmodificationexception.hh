

#pragma once

#include <exception>


namespace cs::editor::model
{

class FSModificationException : public std::exception
{
public:
  FSModificationException(const std::string &message)
    : std::exception(message.c_str())
  {

  }
};

}

