
#pragma once

#include <exception>
#include <string>
#include <filesystem>

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


class IllegalModelTreeStructException : public std::exception
{
public:
  IllegalModelTreeStructException(const std::string &message)
    : std::exception(message.c_str())
  {

  }
};

class DirectoryNodeEmptyException : public std::exception
{
public:
  DirectoryNodeEmptyException(const std::string &message, std::filesystem::path path)
    : std::exception(message.c_str())
    , m_path(path)
  {

  }

  const std::filesystem::path &GetPath() const
  {
    return m_path;
  }

private:
  std::filesystem::path m_path;
};



class FSModificationException : public std::exception
{
public:
  FSModificationException(const std::string &message)
    : std::exception(message.c_str())
  {

  }
};

class FSDirectoryNotEmptyException : public std::exception
{
public:
  FSDirectoryNotEmptyException(const std::string &message, std::filesystem::path path)
    : std::exception(message.c_str())
    , m_path(path)
  {

  }

  const std::filesystem::path &GetPath() const
  {
    return m_path;
  }

private:
  std::filesystem::path m_path;
};

}