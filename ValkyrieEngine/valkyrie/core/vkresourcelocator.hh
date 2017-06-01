#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkstring.hh>

class VKE_API vkResourceLocator
{
public:
  vkResourceLocator(const vkString &encodedResourceName = "");
  vkResourceLocator(const vkString &resourceFile, const vkString &resourceName, const vkString &resourceEntry = "");
  explicit vkResourceLocator(const vkResourceLocator &resource, const vkString &resourceName);
  vkResourceLocator(const vkResourceLocator &other);

  const vkString &GetResourceFile() const;
  const vkString &GetResourceName() const;
  const vkString &GetResourceEntry() const;

  vkString GetDebugName() const;

  bool operator< (const vkResourceLocator &o) const;
  bool operator== (const vkResourceLocator &o) const;
  bool operator!= (const vkResourceLocator &o) const
  {
    return !(*this == o);
  }

  bool IsValid() const;

private:
  void FixResourceFile();
  vkString m_resourceEntry;
  vkString m_resourceFile;
  vkString m_resourceName;

};
