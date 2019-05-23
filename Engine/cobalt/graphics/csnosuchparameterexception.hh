
#pragma once


#include <exception>
#include <cobalt/cstypes.hh>
#include <string>

namespace cs
{


class NoSuchParameterNameException : public std::exception
{
public:
  NoSuchParameterNameException(const std::string& name);

private:
  std::string m_name;
};

class NoSuchParameterIdException : public std::exception
{
public:
  NoSuchParameterIdException(const std::string& id);

private:
  std::string m_id;
};

class NoSuchParameterIndexException : public std::exception
{
public:
  NoSuchParameterIndexException(csSize idx);

private:
  csSize m_idx;
};

}
