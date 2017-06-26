#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csstring.hh>

class CSE_API csResourceLocator
{
public:
  csResourceLocator(const csString &encodedResourceName = "");
  csResourceLocator(const csString &resourceFile, const csString &resourceName, const csString &resourceEntry = "");
  explicit csResourceLocator(const csResourceLocator &resource, const csString &resourceName);
  csResourceLocator(const csResourceLocator &other);

  csResourceLocator AsAnonymous() const;
  csResourceLocator AsFileName() const;

  const csString &GetResourceFile() const;
  const csString &GetResourceName() const;
  const csString &GetResourceEntry() const;

  csString GetText() const;
  csString GetDebugName() const;

  bool operator< (const csResourceLocator &o) const;
  bool operator== (const csResourceLocator &o) const;
  bool operator!= (const csResourceLocator &o) const
  {
    return !(*this == o);
  }

  bool IsValid() const;

private:
  void FixResourceFile();
  csString m_resourceEntry;
  csString m_resourceFile;
  csString m_resourceName;

};