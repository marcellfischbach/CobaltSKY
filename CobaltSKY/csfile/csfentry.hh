#pragma once

#include <csfile/csfexport.hh>
#include <string>
#include <vector>

class CSF_API csfEntry
{
  friend class csfFile;
public:
  ~csfEntry();

  void SetTagName(const std::string &tagName);
  const std::string &GetTagName() const;
  
  size_t GetNumberOfAttributes() const;
  std::string GetAttributeKey(size_t idx) const;
  std::string GetAttributeValue(size_t idx) const;

  void AddAttribute(const std::string &attribute);
  void AddAttribute(const std::string &key, const std::string &value);
  bool HasAttribute(size_t idx) const;
  bool HasAttribute(const std::string &key) const;

  std::string GetAttribute(size_t idx, const std::string &defaultValue = "") const;
  std::string GetAttribute(const std::string &key, const std::string &defaultValue = "") const;
  int GetAttributeInt(size_t idx, int defaultValue = 0) const;
  int GetAttributeInt(const std::string &key, int defaultValue = 0) const;
  long GetAttributeLong(size_t idx, long defaultValue = 0) const;
  long GetAttributeLong(const std::string &key, long defaultValue = 0) const;
  float GetAttributeFloat(size_t idx, float defaultValue = 0.0f) const;
  float GetAttributeFloat(const std::string &key, float defaultValue = 0.0f) const;
  double GetAttributeDouble(size_t idx, double defaultValue = 0.0) const;
  double GetAttributeDouble(const std::string &key, double defaultValue = 0.0) const;


  csfEntry *GetParent();
  void AddChild(csfEntry *child);

  size_t GetNumberOfChildren() const;
  csfEntry *GetChild(size_t idx);
  const csfEntry *GetChild(size_t idx) const;

  const csfEntry *GetEntry(const std::string &entry) const;

  void Debug() const;

  csfFile *GetFile();
  const csfFile *GetFile() const;

private:
  csfEntry(csfFile *file);
  explicit csfEntry(csfFile *file, const std::string &tagName);

  struct Attribute
  {
    std::string key;
    std::string value;
  };

  void Debug(int indent) const;

  std::string m_tagName;

  csfFile *m_file;
  csfEntry *m_parent;
  std::vector<csfEntry*> m_children;

  std::vector<Attribute> m_attributes;


};
