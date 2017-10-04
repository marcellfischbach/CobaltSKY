

#pragma once

#include <cobalt/csexport.hh>
#include <csfile/csffile.hh>
#include <string>


class CSE_API csSettings
{

public:

  static csSettings* Get ();

  bool Initialize(int argc, char** argv);
  bool Initialize(const std::string &configFileName);
  const std::string &GetRootPath () const;


  bool HasEntry(const std::string &entry) const;
  const csfEntry *GetEntry(const std::string &entry) const;

  bool HasValue(const std::string &entry, size_t attribute) const;
  bool HasValue(const std::string &entry, const std::string &attributeName) const;

  std::string GetStringValue(const std::string &entry, size_t attribute, const std::string &defaultValue = std::string("")) const;
  std::string GetStringValue(const std::string &entry, const std::string &attributeName, const std::string &defaultValue = std::string ("")) const;

  int GetIntValue(const std::string &entry, size_t attribute, int defaultValue = 0) const;
  int GetIntValue(const std::string &entry, const std::string &attributeName, int defaultValue = 0) const;

  float GetFloatValue(const std::string &entry, size_t attribute, float defaultValue = 0.0f) const;
  float GetFloatValue(const std::string &entry, const std::string &attributeName, float defaultValue = 0.0f) const;

  bool GetBoolValue(const std::string &entry, size_t attribute, bool defaultValue = false) const;
  bool GetBoolValue(const std::string &entry, const std::string &attributeName, bool defaultValue = false) const;

protected:
  csSettings();

private:
  csfFile m_file;

  static csSettings*  static_instances;
  std::string         _rootPath;


};

