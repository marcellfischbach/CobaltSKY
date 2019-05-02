

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

    std::string sTime0 = line.substr(0, idx);
    line = line.substr(idx + 1);

    idx = line.find(':');
    if (idx == std::string::npos)
    {
      continue;
    }

    std::string sTime1 = line.substr(0, idx);
    line = line.substr(idx + 1);


    idx = line.find(':');
    if (idx == std::string::npos)
    {
      continue;
    }

    std::string sNumClasses = line.substr(0, idx);
    std::string headerName = line.substr(idx + 1);
    int numClasses = std::atoi(sNumClasses.c_str());
    long long time0 = std::atoll(sTime0.c_str());
    long long time1 = std::atoll(sTime1.c_str());

    Data data;
    data.filename = headerName;
    data.cacheTime.dwLowDateTime = time0;
    data.cacheTime.dwHighDateTime = time1;


    std::cout << time0 << "@" << time1 << "@" << numClasses << "@" << headerName << std::endl;


    for (int i = 0; i < numClasses; ++i)
    {
      if (!std::getline(stream, line))
      {
        break;
      }
      data.classes.push_back(line);
    }

    this->m_fileCache[headerName] = data;

  }

}

bool Cache::getFileTime(const std::string& filename, LPFILETIME lpft) const
{
  FILETIME ft;
  HANDLE hFile = CreateFile(filename.c_str(), 
    GENERIC_READ, 
    FILE_SHARE_READ, 
    NULL,
    OPEN_EXISTING, 
    0, 
    NULL);

  if (hFile == INVALID_HANDLE_VALUE)
  {
    return false;
  }

  // https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-getfiletime
  // https://docs.microsoft.com/de-de/windows/desktop/api/fileapi/nf-fileapi-createfilea
  // https://docs.microsoft.com/de-de/windows/desktop/SysInfo/retrieving-the-last-write-time
}


}