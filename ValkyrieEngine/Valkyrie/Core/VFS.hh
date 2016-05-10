
#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/IFile.hh>
#include <Valkyrie/Core/String.hh>
#include <map>


using std::map;


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
  static vkVFS* Get ();

  bool Initialize(int argc, char** argv);
  void SetRootPath (const vkString& rootPath);
  const vkString &GetRootPath()const;

  bool HasPath (const vkString &vfsName) const;
  void AddPath (const vkString &vfsName, const vkString &path);
  const vkString &GetPath (const vkString &vfsName) const;
  void RemovePath (const vkString &vfsName);

  vkString GetPathResolution (const vkString &path) const;


  /**
  * \name Functions for opening files
  * @{
  */
  IFile* Open (const vkString& filename, vkOpenMode mode = eOM_Read, vkTextMode textMode = eTM_Binary);
  /**
  * @}
  */

private:
  vkVFS ();

  static vkString ExtractSymbol (const vkString &path, unsigned idx, unsigned &length);

  
  std::string		m_rootPath;
  map<vkString, vkString> m_pathMapping;

  vkString m_illegalPath;

};

/** @} */


