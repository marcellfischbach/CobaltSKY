

#pragma once

#include <cobalt/csexport.hh>
#include <csini/csini.hh>
#include <string>


class CSE_API csSettings
{

public:

  static csSettings* Get ();

  bool Initialize(int argc, char** argv);
  bool Initialize(const std::string &configFileName);
  const std::string &GetRootPath () const;

  std::vector<std::string> FindSubGroupNames(const std::string &subGroup) const;

  bool HasGroup (const std::string &group);

  bool HasValue(const std::string &entry = 0);
  std::string GetStringValue(const std::string &entry = 0);
  int GetIntValue(const std::string &entry = 0, int defaultValue = 0);
  float GetFloatValue(const std::string &entry = 0, float defaultValue = 0.0f);
  bool GetBoolValue(const std::string &entry = 0, bool defaultValue = false);

  bool HasValue (const std::string &group, const std::string &entry = 0);
  std::string GetStringValue (const std::string &group, const std::string &entry = 0);
  int GetIntValue (const std::string &group, const std::string &entry = 0, int defaultValue = 0);
  float GetFloatValue (const std::string &group, const std::string &entry = 0, float defaultValue = 0.0f);
  bool GetBoolValue (const std::string &group, const std::string &entry = 0, bool defaultValue = false);

protected:
  csSettings();

private:
  csINI m_ini;

  static csSettings*  static_instances;
  std::string         _rootPath;


};

