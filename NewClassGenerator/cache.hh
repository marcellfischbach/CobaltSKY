
#pragma once

#include <string>
#include <map>
#include <vector>

#include <Windows.h>

namespace cs::classgenerator
{


class Cache
{
public:
  void Load(const std::string& path);

private:

  struct Data 
  {
    std::string filename;
    FILETIME filetime;
    FILETIME cacheTime;
    std::vector<std::string> classes;
  };

  bool getFileTime(const std::string& filename, LPFILETIME lpft) const;

  std::map<std::string, Data> m_fileCache;
};

}