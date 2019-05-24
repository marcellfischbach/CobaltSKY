#pragma once

#include <csfile/csfexport.hh>
#include <cobalt/csdefs.hh>
#include <string>
#include <vector>



namespace cs::file
{
class File;

class CSF_API Entry
{
  friend class cs::file::File;
public:
  static const size_t InvalidIndex = ~0x00;


  void SetTagName(const std::string& tagName);
  const std::string& GetTagName() const;

  void RemoveAttributes();
  void RemoveAttribute(size_t idx);
  size_t GetNumberOfAttributes() const;
  std::string GetAttributeKey(size_t idx) const;
  std::string GetAttributeValue(size_t idx) const;
  size_t GetAttributeIndex(const std::string& name) const;

  cs::file::Entry* AddAttribute(const std::string& attribute);
  cs::file::Entry* AddAttribute(const std::string& key, const std::string& value);
  cs::file::Entry* AddAttributeInt(int attribute);
  cs::file::Entry* AddAttributeInt(const std::string& key, int attribute);
  cs::file::Entry* AddAttributeLong(long attribute);
  cs::file::Entry* AddAttributeLong(const std::string& key, long attribute);
  cs::file::Entry* AddAttributeFloat(float attribute);
  cs::file::Entry* AddAttributeFloat(const std::string& key, float attribute);
  cs::file::Entry* AddAttributeDouble(double attribute);
  cs::file::Entry* AddAttributeDouble(const std::string& key, double attribute);
  bool HasAttribute(size_t idx = 0) const;
  bool HasAttribute(const std::string& key) const;


  inline std::string GetAttribute() const
  {
    return GetAttribute(0, std::string(""));
  }
  inline std::string GetAttribute(size_t idx) const
  {
    return GetAttribute(idx, std::string(""));
  }
  std::string GetAttribute(size_t idx, const std::string& defaultValue) const;

  inline std::string GetAttribute(const std::string& key) const
  {
    return GetAttribute(key, std::string(""));
  }
  std::string GetAttribute(const std::string& key, const std::string& defaultValue) const;

  int GetAttributeInt(size_t idx = 0, int defaultValue = 0) const;
  int GetAttributeInt(const std::string& key, int defaultValue = 0) const;
  long GetAttributeLong(size_t idx = 0, long defaultValue = 0) const;
  long GetAttributeLong(const std::string& key, long defaultValue = 0) const;
  float GetAttributeFloat(size_t idx = 0, float defaultValue = 0.0f) const;
  float GetAttributeFloat(const std::string& key, float defaultValue = 0.0f) const;
  double GetAttributeDouble(size_t idx = 0, double defaultValue = 0.0) const;
  double GetAttributeDouble(const std::string& key, double defaultValue = 0.0) const;

  bool IsRoot() const;
  cs::file::Entry* GetParent();
  const cs::file::Entry* GetParent() const;
  cs::file::Entry* AddChild(cs::file::Entry* child);
  cs::file::Entry* RemoveChild(cs::file::Entry* child);
  void RemoveAllChildren();

  size_t GetNumberOfChildren() const;
  cs::file::Entry* GetChild(size_t idx);
  const cs::file::Entry* GetChild(size_t idx) const;
  size_t GetChildIndex(const cs::file::Entry* child) const;

  CS_FORCEINLINE cs::file::Entry* GetEntry(size_t idx = 0) { return GetChild(idx); }
  CS_FORCEINLINE const cs::file::Entry* GetEntry(size_t idx = 0) const { return GetChild(idx); }

  cs::file::Entry* GetEntry(const std::string& entry, const cs::file::Entry* offset = 0);
  const cs::file::Entry* GetEntry(const std::string& entry, const cs::file::Entry* offset = 0) const;

  const cs::file::Entry* GetSiblingEntry(size_t offset = 0) const;
  const cs::file::Entry* GetSiblingEntry(const std::string& entry) const;

  void Debug() const;

  cs::file::File* GetFile();
  const cs::file::File* GetFile() const;

private:
  Entry(cs::file::File* file);
  explicit Entry(cs::file::File* file, const std::string& tagName);

  struct Attribute
  {
    std::string key;
    std::string value;
  };

  void Debug(int indent) const;

  std::string m_tagName;

  cs::file::File* m_file;
  cs::file::Entry* m_parent;
  std::vector<cs::file::Entry*> m_children;

  std::vector<Attribute> m_attributes;


};

}

