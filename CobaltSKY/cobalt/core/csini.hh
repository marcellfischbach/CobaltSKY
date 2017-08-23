
#pragma once

#include <cobalt/csexport.hh>
#include <map>
#include <string>
#include <vector>

class CSE_API csINI
{
public:
  csINI();

  bool Import(const std::string &fileName);
  std::vector<std::string> FindSubGroupNames(const std::string &groupSubName) const;

  bool HasGroup(const std::string &group) const;

  bool HasEntry(const std::string &entry) const;
  std::string GetValue(const std::string &entry) const;
  int GetIntValue(const std::string &entry, int defaultValue = 0) const;
  float GetFloatValue(const std::string &entry, float defaultValue = 0.0f) const;
  bool GetBoolValue(const std::string &entry, bool defaultValue = false) const;


  bool HasEntry (const std::string &group, const std::string &entry) const;
  std::string GetValue(const std::string &group, const std::string &entry) const;
  int GetIntValue(const std::string &group, const std::string &entry, int defaultValue = 0) const;
  float GetFloatValue(const std::string &group, const std::string &entry, float defaultValue = 0.0f) const;
  bool GetBoolValue(const std::string &group, const std::string &entry, bool defaultValue = false) const;


  void Debug() const;

private:
  struct Entry
  {
    std::string key;
    std::string value;
  };

  struct Group
  {
    std::string name;
    std::vector<Entry> entries;
  };

  std::map<std::string, Group> m_groups;

private:
  const Group *GetGroup(const std::string &group) const;
  const Entry *GetEntry(const Group *group, const std::string &entry) const;
  std::string GetEntryValue (const Group *group, const std::string &entry) const;

};