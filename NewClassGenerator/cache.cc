

#include <cache.hh>

#include <iostream>
#include <fstream>
#include <iomanip>

#include <Windows.h>


namespace cs::classgenerator
{


void Cache::Load(const std::string& path)
{
  std::ifstream stream(path + "/test.cscache");
  std::string line;
  int l = 1;
  while (std::getline(stream, line))
  {
    size_t idx = line.find(':');
    if (idx == std::string::npos)
    {
      continue;
    }

    std::string time0 = line.substr(0, idx);
    line = line.substr(idx + 1);

    idx = line.find(':');
    if (idx == std::string::npos)
    {
      continue;
    }

    std::string time1 = line.substr(0, idx);
    line = line.substr(idx + 1);


    idx = line.find(':');
    if (idx == std::string::npos)
    {
      continue;
    }

    std::string numClasses = line.substr(0, idx);
    std::string className = line.substr(idx + 1);


    std::cout << time0 << "@" << time1 << "@" << numClasses << "@" << className << std::endl;
  }

}


}