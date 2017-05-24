
#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/ifile.hh>
#include <valkyrie/core/vkstring.hh>
#include <map>
#include <vector>


using std::map;
using std::vector;


class csSettings;

/**
* \addtogroup engine
* @{
*/

/**
* @brief A simple virtual file system.
*/
class VKE_API vkVFS
{
public:
  class VKE_API Entry
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

  static vkVFS* Get ();

  bool Initialize(int argc, char** argv);

  bool HasPath (const vkString &vfsName) const;
  void AddPath (const vkString &vfsName, const vkString &path);
  const vkString &GetPath (const vkString &vfsName) const;
  void RemovePath (const vkString &vfsName);

  vkString GetPathResolution (const vkString &path) const;


  /**
  * \name Functions for opening files
  * @{
  */
  iFile* Open (const vkString& filename, vkOpenMode mode = eOM_Read, vkTextMode textMode = eTM_Binary);
  /**
  * @}
  */
  vkString GetAbsolutPath(const vkString &path);

  vkSize GetNumberOfEntries () const;
  const Entry &GetEntry (vkSize idx) const;

private:
  vkVFS ();

  static vkString ExtractSymbol (const vkString &path, unsigned idx, unsigned &length);

  
  map<vkString, vkString> m_pathMapping;

  vkString m_illegalPath;

  vector<Entry> m_entries;
};

/** @} */


