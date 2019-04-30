
#pragma once

#include <string>
#include <map>

#include <Windows.h>

namespace cs::classgenerator
{


class Cache
{
public:
  void Load(const std::string& path);

private:

  std::map<std::string, FILETIME> m_fileCache;
};

}