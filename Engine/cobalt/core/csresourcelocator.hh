#pragma once

#include <cobalt/csexport.hh>
#include <string>

class csResourceName
{
public:
  explicit csResourceName(const std::string &name)
    : m_name(name)
  {

  }

  const std::string &GetName() const
  {
    return m_name;
  }
private:
  std::string m_name;
};

class csResourceEntry
{
public:
  explicit csResourceEntry(const std::string &name)
    : m_name(name)
  {

  }

  const std::string &GetName() const
  {
    return m_name;
  }
private:
  std::string m_name;
};

class csResourceFile
{
public:
  explicit csResourceFile(const std::string &name)
    : m_name(name)
  {

  }

  const std::string &GetName() const
  {
    return m_name;
  }
private:
  std::string m_name;
};



class CSE_API csResourceLocator
{
public:
  csResourceLocator(const std::string &encodedResourceName = "");
  csResourceLocator(const csResourceFile &file);
  csResourceLocator(const csResourceFile &file, const csResourceName &name);
  csResourceLocator(const csResourceEntry &entry, const csResourceFile &file);
  csResourceLocator(const csResourceEntry &entry, const csResourceFile &file, const csResourceName &name);

//  csResourceLocator(const std::string &resourceFile, const std::string &resourceName, const std::string &resourceEntry = "");
//  explicit csResourceLocator(const csResourceLocator &resource, const std::string &resourceName);
  csResourceLocator(const csResourceLocator &other);

  csResourceLocator AsAnonymous() const;
  csResourceLocator AsFileName() const;
  csResourceLocator WithFileSuffix(const std::string &suffix) const;
	csResourceLocator WithResourceName(const std::string &resourceName) const;

  bool IsAnonymous() const;

  const std::string &GetResourceFile() const;
  const std::string &GetResourceName() const;
  const std::string &GetResourceEntry() const;

  std::string Encode() const;
  std::string GetDebugName() const;

  bool Equals(const csResourceLocator &other) const;
  /**
   * \brief Is any locator is anonymous test the base locator
   */
  bool EqualsAnonymous(const csResourceLocator &other) const;

  bool operator< (const csResourceLocator &o) const;
  bool operator== (const csResourceLocator &o) const;
  bool operator!= (const csResourceLocator &o) const
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
