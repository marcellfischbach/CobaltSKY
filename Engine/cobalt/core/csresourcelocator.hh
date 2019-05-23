#pragma once

#include <cobalt/csexport.hh>
#include <string>

namespace cs
{

class ResourceName
{
public:
  explicit ResourceName(const std::string& name)
    : m_name(name)
  {

  }

  const std::string& GetName() const
  {
    return m_name;
  }
private:
  std::string m_name;
};

class ResourceEntry
{
public:
  explicit ResourceEntry(const std::string& name)
    : m_name(name)
  {

  }

  const std::string& GetName() const
  {
    return m_name;
  }
private:
  std::string m_name;
};

class ResourceFile
{
public:
  explicit ResourceFile(const std::string& name)
    : m_name(name)
  {

  }

  const std::string& GetName() const
  {
    return m_name;
  }
private:
  std::string m_name;
};



class CSE_API ResourceLocator
{
public:
  ResourceLocator(const std::string& encodedResourceName = "");
  ResourceLocator(const cs::ResourceFile& file);
  ResourceLocator(const cs::ResourceEntry& entry);
  ResourceLocator(const cs::ResourceFile& file, const cs::ResourceName& name);
  ResourceLocator(const cs::ResourceEntry& entry, const cs::ResourceFile& file);
  ResourceLocator(const cs::ResourceEntry& entry, const cs::ResourceFile& file, const cs::ResourceName& name);

  static cs::ResourceLocator Invalid();

  //  cs::ResourceLocator(const std::string &resourceFile, const std::string &resourceName, const std::string &resourceEntry = "");
  //  explicit cs::ResourceLocator(const cs::ResourceLocator &resource, const std::string &resourceName);
  ResourceLocator(const cs::ResourceLocator& other);

  cs::ResourceLocator AsAnonymous() const;
  cs::ResourceLocator AsFileName() const;
  cs::ResourceLocator WithFileSuffix(const std::string& suffix) const;
  cs::ResourceLocator WithResourceName(const std::string& resourceName) const;

  bool IsAnonymous() const;

  const std::string& GetResourceFile() const;
  const std::string& GetResourceName() const;
  const std::string& GetResourceEntry() const;

  std::string Encode() const;
  std::string GetDebugName() const;

  bool Equals(const cs::ResourceLocator& other) const;
  /**
   * \brief Is any locator is anonymous test the base locator
   */
  bool EqualsAnonymous(const cs::ResourceLocator& other) const;

  bool operator< (const cs::ResourceLocator& o) const;
  bool operator== (const cs::ResourceLocator& o) const;
  bool operator!= (const cs::ResourceLocator& o) const
  {
    return !(*this == o);
  }

  bool IsValid() const;


private:

  void FixResourceFile();
  std::string m_resourceEntry;
  std::string m_resourceFile;
  std::string m_resourceName;
};


}

