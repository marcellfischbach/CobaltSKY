#pragma once

#include <string>
#include <Windows.h>
#include <list>

class ProcessedFilesCache
{
public:
  struct CacheFile
  {
    std::string filename;
    std::string outHeaderFilename;
    std::string outSourceFilename;
    size_t numClasses;
    FILETIME filetime;
    bool touched;
  };

public:
  ProcessedFilesCache();

  void Init(const std::string &cacheFilename);
  void Output();

  /**
   * \return \a True if the file needs to be processed else \a false
   */
  bool TouchFile(const std::string &filename);
  void UpdateFile(const std::string &filename, const std::string &outHeaderFilename, const std::string &outSourceFilename, size_t numClasses);

  std::list<CacheFile> GetUntouchedFiles() const;
  std::list<CacheFile> GetTouchedFiles() const;

private:

  std::list<CacheFile> m_files;
  std::string m_cacheFilename;
};


class KnownClassesCache
{
public:
  struct CacheClass
  {
    std::string filename;
    std::string className;
  };

public:
  KnownClassesCache();

  void Init(const std::string &cacheFilename);
  void Output();

  int RemoveAllClasses(const std::string &filename);
  void Add(const std::string &filename, const std::string &className);

  const std::list<CacheClass>& GetClasses() const
  {
    return m_classes;
  }

  std::list<CacheClass> GetClasses(const std::string &filename) const;

  bool ContainsClass(const std::string &className) const;

  std::list<std::string> GetAllClasses() const;
private:
  std::list<CacheClass> m_classes;
  std::string m_cacheFilename;
};