#pragma once

#include <cobalt/csexport.hh>
#include <exception>
#include <string>

namespace cs
{


class Exception : public std::exception
{
public:
  Exception();
  Exception(const std::string& what);
  virtual ~Exception();
  virtual const char* what() const noexcept
  {
    return m_what.c_str();
  }

private:
  std::string m_what;
};





}



inline cs::Exception::Exception()
  : std::exception()
  , m_what("")
{

}

inline cs::Exception::Exception(const std::string& what)
  : std::exception()
  , m_what(what)
{

}


inline cs::Exception::~Exception()
{

}
