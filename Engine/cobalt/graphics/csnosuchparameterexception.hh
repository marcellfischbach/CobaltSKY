
#pragma once


#include <exception>
#include <cobalt/cstypes.hh>
#include <string>


class csNoSuchParameterNameException : public std::exception
{
public:
  csNoSuchParameterNameException(const std::string &name);

private:
  std::string m_name;
};

class csNoSuchParameterIdException : public std::exception
{
public:
  csNoSuchParameterIdException(const std::string &id);

private:
  std::string m_id;
};

class csNoSuchParameterIndexException : public std::exception
{
public:
  csNoSuchParameterIndexException(csSize idx);

private:
  csSize m_idx;
};
