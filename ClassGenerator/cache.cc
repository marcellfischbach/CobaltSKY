#pragma warning(disable: 4996)

#include "cache.hh"
#include <vector>
#include <iostream>


ProcessedFilesCache::ProcessedFilesCache()
{

}


#pragma pack(1)
struct CacheFileEntry
{
  DWORD lowFileTime;
  DWORD highFileTime;
  unsigned numClasses;
  char filename[1024];
};


void ProcessedFilesCache::Init(const std::string &cacheFilename)
{
  m_cacheFilename = cacheFilename;
  m_files.clear();

  FILE *file;
  fopen_s(&file, cacheFilename.c_str(), "rb");
  if (!file)
  {
    // there is no cache file... but thats ok
    return;
  }
  

  unsigned numFiles;
  fread(&numFiles, sizeof(unsigned), 1, file);
  CacheFileEntry *entries = new CacheFileEntry[numFiles];
  fread(entries, sizeof(CacheFileEntry), numFiles, file);
  fclose(file);

  for (unsigned i = 0; i < numFiles; ++i)
  {
    CacheFileEntry &entry = entries[i];

    CacheFile file;
    file.filetime.dwLowDateTime = entry.lowFileTime;
    file.filetime.dwHighDateTime = entry.highFileTime;
    file.filename = std::string(entry.filename);
    file.numClasses = 
    file.touched = false;

    m_files.push_back(file);
  }

  delete[] entries;
}

void ProcessedFilesCache::Output()
{
  std::vector<CacheFileEntry> entries;
  for (auto it = m_files.begin(); it != m_files.end(); ++it)
  { 
    CacheFile &cf = *it;
    if (cf.touched)
    {
      CacheFileEntry entry;
      memset(&entry, 0, sizeof(CacheFileEntry));
      memcpy(entry.filename, cf.filename.c_str(), cf.filename.length());
      entry.lowFileTime = cf.filetime.dwLowDateTime;
      entry.highFileTime = cf.filetime.dwHighDateTime;
      entries.push_back(entry);
    }
  }

  FILE *file;
  fopen_s(&file, m_cacheFilename.c_str(), "wb");
  if (!file)
  {
    // there is no cache file... buts thats ok
    return;
  }

  unsigned num = (unsigned)entries.size();
  fwrite(&num, sizeof(unsigned), 1, file);
  fwrite(entries.data(), sizeof(CacheFileEntry), num, file);
  fclose(file);
}

unsigned touchedFiles = 0;

bool ProcessedFilesCache::TouchFile(const std::string &filename)
{
  FILETIME ft;
  HANDLE handle = CreateFile(filename.c_str(),
                             GENERIC_READ,
                             0,
                             0,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             0);
  if (handle)
  {
    if (!GetFileTime(handle, 0, 0, &ft))
    {
      CloseHandle(handle);
      return false;
    }
    CloseHandle(handle);
  }
  else
  {
    return false;
  }


  for (std::list<CacheFile>::iterator it = m_files.begin(); it != m_files.end(); ++it)
  {
    CacheFile &cf = *it;
    if (filename == cf.filename)
    {
      cf.touched = true;
      if (ft.dwHighDateTime != cf.filetime.dwHighDateTime ||
          ft.dwLowDateTime != cf.filetime.dwLowDateTime)
      {
        cf.filetime.dwHighDateTime = ft.dwHighDateTime;
        cf.filetime.dwLowDateTime = ft.dwLowDateTime;
        return true;
      }
      return false;
    }
  }

  CacheFile cf;
  cf.filename = filename;
  cf.outHeaderFilename = "";
  cf.outSourceFilename = "";
  cf.filetime.dwHighDateTime = ft.dwHighDateTime;
  cf.filetime.dwLowDateTime = ft.dwLowDateTime;
  cf.numClasses = 0;
  cf.touched = true;

  m_files.push_back(cf);
  return true;
}

void ProcessedFilesCache::UpdateFile(const std::string &filename, const std::string &outHeaderFilename, const std::string &outSourceFilename, size_t numClasses)
{
  
  for (std::list<CacheFile>::iterator it = m_files.begin(); it != m_files.end(); ++it)
  {
    CacheFile &cf = *it;
    if (filename == cf.filename)
    {
      cf.outHeaderFilename = outHeaderFilename;
      cf.outSourceFilename = outSourceFilename;
      cf.numClasses = numClasses;
      return;
    }
  }
}



std::list<ProcessedFilesCache::CacheFile> ProcessedFilesCache::GetUntouchedFiles() const
{
  std::list<CacheFile> result;
  for (CacheFile cf : m_files)
  {
    if (!cf.touched)
    {
      result.push_back(cf);
    }
  }
  return result;
}


std::list<ProcessedFilesCache::CacheFile> ProcessedFilesCache::GetTouchedFiles() const
{
  std::list<CacheFile> result;
  for (CacheFile cf : m_files)
  {
    if (cf.touched)
    {
      result.push_back(cf);
    }
  }
  return result;
}












#pragma pack(1)
struct CacheClassEntry
{
  char filename[1024];
  char classname[1024];
};


KnownClassesCache::KnownClassesCache()
{

}


void KnownClassesCache::Init(const std::string &cacheFilename)
{
  m_cacheFilename = cacheFilename;
  m_classes.clear();

  FILE *file;
  fopen_s(&file, cacheFilename.c_str(), "rb");
  if (!file)
  {
    // there is no cache yet... thats ok
    return;
  }

  unsigned num;
  fread(&num, sizeof(unsigned), 1, file);
  CacheClassEntry *entries = new CacheClassEntry[num];
  fread(entries, sizeof(CacheClassEntry), num, file);
  fclose(file);

  for (unsigned i = 0; i < num; ++i)
  {
    CacheClass cc;
    cc.className = entries[i].classname;
    cc.filename = entries[i].filename;
    m_classes.push_back(cc);
  }

  delete[] entries;
}

void KnownClassesCache::Output()
{
  std::vector<CacheClassEntry> entries (m_classes.size());
  int i = 0;
  for (auto it = m_classes.begin(); it != m_classes.end(); ++it)
  {
    CacheClassEntry &entry = entries[i++];
    memset(&entry, 0, sizeof(CacheClassEntry));
    memcpy(entry.classname, it->className.c_str(), it->className.size());
    memcpy(entry.filename, it->filename.c_str(), it->filename.size());
  }

  FILE *file;
  fopen_s(&file, m_cacheFilename.c_str(), "wb");
  if (!file)
  {
    // there is no cache file... buts thats ok
    return;
  }

  std::cout << "ClassCache has " << entries.size() << " entries" << std::endl;
  unsigned num = (unsigned)entries.size();
  fwrite(&num, sizeof(unsigned), 1, file);
  fwrite(entries.data(), sizeof(CacheClassEntry), num, file);
  fclose(file);

}

int KnownClassesCache::RemoveAllClasses(const std::string &filename)
{
  int count = 0;
  for (auto it = m_classes.begin(); it != m_classes.end();)
  {
    if (it->filename == filename)
    {
      it = m_classes.erase(it);
      ++count;
    }
    else
    {
      ++it;
    }
  }
  return count;
}

void KnownClassesCache::Add(const std::string &fileName, const std::string &className)
{
  CacheClass cc;
  cc.filename = fileName;
  cc.className = className;
  m_classes.push_back(cc);
}


bool KnownClassesCache::ContainsClass(const std::string &className) const
{
  if (className == "iObject" || className == "csObject")
  {
    // instances of iObject or csObject are actually not within the cache...
    // but they are ofcourse implementations of iObject
    return true;
  }

  for (auto it = m_classes.begin(); it != m_classes.end(); ++it)
  {
    if (it->className == className)
    {
      return true;
    }
  }
  return false;
}


std::list<KnownClassesCache::CacheClass> KnownClassesCache::GetClasses(const std::string &filename) const
{
  std::list<CacheClass> classes;
  for (auto it = m_classes.begin(); it != m_classes.end(); ++it)
  {
    if (it->filename == filename)
    {
      classes.push_back(*it);
    }
  }

  return classes;
}

std::list<std::string> KnownClassesCache::GetAllClasses() const
{
  std::list<std::string> names;
  for (auto it = m_classes.begin(); it != m_classes.end(); ++it)
  {
    names.push_back(it->className);
  }

  return names;
}
