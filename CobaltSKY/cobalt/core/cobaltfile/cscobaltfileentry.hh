#pragma once

#include <string>
#include <vector>

class csCobaltFileEntry
{
public:
  csCobaltFileEntry();
  explicit csCobaltFileEntry(const std::string &tagName);
  ~csCobaltFileEntry();

  void SetTagName(const std::string &tagName);
  const std::string &GetTagName() const;

  void AddAttribute(const std::string &attribute);
  void AddAttribute(const std::string &key, const std::string &value);
  bool HasAttribute(size_t idx) const;
  bool HasAttribute(const std::string &key) const;
  std::string GetAttribute(size_t idx) const;
  std::string GetAttribute(const std::string &key) const;

  int GetAttributeInt(size_t idx) const;
  int GetAttributeInt(const std::string &key) const;
  long GetAttributeLong(size_t idx) const;
  long GetAttributeLong(const std::string &key) const;
  float GetAttributeFloat(size_t idx) const;
  float GetAttributeFloat(const std::string &key) const;
  double GetAttributeDouble(size_t idx) const;
  double GetAttributeDouble(const std::string &key) const;



  csCobaltFileEntry *GetParent();
  void AddChild(csCobaltFileEntry *child);

  void Debug() const;

private:

  struct Attribute
  {
    std::string key;
    std::string value;
  };

  void Debug(int indent) const;

  std::string m_tagName;

  csCobaltFileEntry *m_parent;
  std::vector<csCobaltFileEntry*> m_children;

  std::vector<Attribute> m_attributes;

};
