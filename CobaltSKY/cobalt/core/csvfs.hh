
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csstring.hh>
#include <map>
#include <vector>


using std::map;
using std::vector;


class csSettings;
class TiXmlElement;

/**
* \addtogroup engine
* @{
*/

/**
* @brief A simple virtual file system.
*/
class CSE_API csVFS
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

  static csVFS* Get ();

  bool LoadConfig(const std::string &configFilename);
  bool Initialize(int argc, char** argv);
  bool Initialize(csSettings *settings);

  bool HasPath (const csString &vfsName) const;
  void AddPath (const csString &vfsName, const csString &path);
  const csString &GetPath (const csString &vfsName) const;
  void RemovePath (const csString &vfsName);

  csString GetPathResolution (const csString &path) const;
  csString GetAbsolutePath(const csString &path) const;
  csString GetAbsolutePath(const csString &path, const csString &entryName, ExistenceCheck checkExistence = CheckExistence) const;
  csString GetAbsolutePath(const csResourceLocator &locator, ExistenceCheck checkExistence = CheckExistence) const;
  /**
  * \name Functions for opening files
  * @{
  */
  iFile* Open(const csString& filename, csOpenMode mode = eOM_Read, csTextMode textMode = eTM_Binary);
  iFile* Open(const csString& filename, const csString &entryName, csOpenMode mode = eOM_Read, csTextMode textMode = eTM_Binary);
  iFile *Open(const csResourceLocator &locator, csOpenMode mode = eOM_Read, csTextMode textMode = eTM_Binary);
  /**
  * @}
  */

  const Entry *FindEntryForFilename(const std::string &filename) const;

  csSize GetNumberOfEntries () const;
  const Entry &GetEntry (csSize idx) const;

private:
  csVFS ();
  bool LoadConfig(const TiXmlElement *vfsElement, const std::string &basePath);

  static csString ExtractSymbol (const csString &path, unsigned idx, unsigned &length);

  
  map<csString, csString> m_pathMapping;

  csString m_illegalPath;

  vector<Entry> m_entries;
};

/** @} */

