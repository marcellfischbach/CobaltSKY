#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkstring.hh>

class VKE_API vkResourceLocator
{
public:
  vkResourceLocator(const vkString &resourceFile = "", const vkString &resourceName = "");
  explicit vkResourceLocator(const vkResourceLocator &resource, const vkString &resourceName);

  const vkString &GetResourceFile() const;
  const vkString &GetResourceName() const;

  vkString GetDebugName() const;

  bool operator< (const vkResourceLocator &o) const;
  bool operator== (const vkResourceLocator &o) const;
  bool operator!= (const vkResourceLocator &o) const
  {
    return !(*this == o);
  }

  bool IsValid() const;

private:
  vkString m_resourceFile;
  vkString m_resourceName;

};
