#pragma once

#include <csfile/csfexport.hh>
#include <cobalt/csdefs.hh>
#include <string>
#include <vector>


class csfFile;

class CSF_API csfEntry
{
  friend class csfFile;
public:
  static const size_t InvalidIndex = ~0x00;


  void SetTagName(const std::string &tagName);
  const std::string &GetTagName() const;

  void RemoveAttributes();
  size_t GetNumberOfAttributes() const;
  std::string GetAttributeKey(size_t idx) const;
  std::string GetAttributeValue(size_t idx) const;
  size_t GetAttributeIndex(const std::string &name) const;

  csfEntry *AddAttribute(const std::string &attribute);
  csfEntry *AddAttribute(const std::string &key, const std::string &value);
  csfEntry *AddAttributeInt(int attribute);
  csfEntry *AddAttributeInt(const std::string &key, int attribute);
  csfEntry *AddAttributeLong(long attribute);
  csfEntry *AddAttributeLong(const std::string &key, long attribute);
  csfEntry *AddAttributeFloat(float attribute);
  csfEntry *AddAttributeFloat(const std::string &key, float attribute);
  csfEntry *AddAttributeDouble(double attribute);
  csfEntry *AddAttributeDouble(const std::string &key, double attribute);
  bool HasAttribute(size_t idx = 0) const;
  bool HasAttribute(const std::string &key) const;

  std::string GetAttribute(size_t idx = 0, const std::string &defaultValue = "") const;
  std::string GetAttribute(const std::string &key, const std::string &defaultValue = "") const;
  int GetAttributeInt(size_t idx = 0, int defaultValue = 0) const;
  int GetAttributeInt(const std::string &key, int defaultValue = 0) const;
  long GetAttributeLong(size_t idx = 0, long defaultValue = 0) const;
  long GetAttributeLong(const std::string &key, long defaultValue = 0) const;
  float GetAttributeFloat(size_t idx = 0, float defaultValue = 0.0f) const;
  float GetAttributeFloat(const std::string &key, float defaultValue = 0.0f) const;
  double GetAttributeDouble(size_t idx = 0, double defaultValue = 0.0) const;
  double GetAttributeDouble(const std::string &key, double defaultValue = 0.0) const;

  bool IsRoot() const;
  csfEntry *GetParent();
  const csfEntry *GetParent() const;
  csfEntry *AddChild(csfEntry *child);
  csfEntry *RemoveChild(csfEntry *child);
  void RemoveAllChildren ();

  size_t GetNumberOfChildren() const;
  csfEntry *GetChild(size_t idx);
  const csfEntry *GetChild(size_t idx) const;
  size_t GetChildIndex(const csfEntry *child) const;

  CS_FORCEINLINE csfEntry *GetEntry(size_t idx = 0) { return GetChild(idx);  }
  CS_FORCEINLINE const csfEntry *GetEntry(size_t idx = 0) const { return GetChild(idx); }

  csfEntry *GetEntry(const std::string &entry, const csfEntry *offset = 0);
  const csfEntry *GetEntry(const std::string &entry, const csfEntry *offset = 0) const;

  const csfEntry *GetSiblingEntry(size_t offset = 0) const;
  const csfEntry *GetSiblingEntry(const std::string &entry) const;

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
