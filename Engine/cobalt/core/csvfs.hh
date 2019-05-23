
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <map>
#include <vector>
#include <string>


using std::map;
using std::vector;


namespace cs
{
class Settings;

namespace file
{
class Entry;
}

/**
* \addtogroup engine
* @{
*/

/**
* @brief A simple virtual file system.
*/
class CSE_API VFS
{
public:
  class CSE_API Entry
  {
  public:
    Entry ();

    void SetName (const std::string &name);
    const std::string &GetName () const;

    void SetPath(const std::string &path);
    const std::string &GetPath () const;

    void SetAbsPath(const std::string &absPath);
    const std::string &GetAbsPath () const;

    void SetPriority (int priority);
    int GetPriority () const;

  private:
    std::string m_name;
    std::string m_path;
    std::string m_absPath;
    int m_priority;
  };

  enum ExistenceCheck
  {
    CheckExistence,
    DontCheckExistence,
  };

  static cs::VFS* Get ();

  bool LoadConfig(const std::string &configFilename);
  bool Initialize(int argc, char** argv);
  bool Initialize(cs::Settings *settings);

  bool HasPath (const std::string &vfsName) const;
  void AddPath (const std::string &vfsName, const std::string &path);
  const std::string &GetPath (const std::string &vfsName) const;
  void RemovePath (const std::string &vfsName);

  std::string GetPathResolution (const std::string &path) const;
  std::string GetAbsolutePath(const std::string &path) const;
  std::string GetAbsolutePath(const std::string &path, const std::string &entryName, ExistenceCheck checkExistence = CheckExistence) const;
  std::string GetAbsolutePath(const cs::ResourceLocator &locator, ExistenceCheck checkExistence = CheckExistence) const;
  /**
  * \name Functions for opening files
  * @{
  */
  cs::iFile* Open(const std::string& filename, cs::eOpenMode mode = eOM_Read, cs::eTextMode textMode = eTM_Binary);
  cs::iFile* Open(const std::string& filename, const std::string &entryName, cs::eOpenMode mode = eOM_Read, cs::eTextMode textMode = eTM_Binary);
  cs::iFile *Open(const cs::ResourceLocator &locator, cs::eOpenMode mode = eOM_Read, cs::eTextMode textMode = eTM_Binary);
  /**
  * @}
  */

  const Entry *FindEntryForFilename(const std::string &filename) const;

  csSize GetNumberOfEntries () const;
  const Entry &GetEntry (csSize idx) const;

private:
  VFS();
  bool LoadConfig(const cs::file::Entry *vfsEntry, const std::string &basePath);
  bool ImportRootPath(cs::Settings *settings, const cs::file::Entry *rootPathEntry);
  bool ImportResolution(cs::Settings *settings, const cs::file::Entry *resolutionEntry);

  static std::string ExtractSymbol (const std::string &path, unsigned idx, unsigned &length);

  
  map<std::string, std::string> m_pathMapping;

  std::string m_illegalPath;

  vector<Entry> m_entries;
};

/** @} */


}
