#pragma once

#include <cobalt/csexport.hh>
#include <string>

class CSE_API csResourceLocator
{
public:
  csResourceLocator(const std::string &encodedResourceName = "");
  csResourceLocator(const std::string &resourceFile, const std::string &resourceName, const std::string &resourceEntry = "");
  explicit csResourceLocator(const csResourceLocator &resource, const std::string &resourceName);
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
