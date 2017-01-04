#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkstring.hh>

class VKE_API vkResourceLocator
{
public:
  vkResourceLocator(const vkString &resourceFile = "", const vkString &resourceName = "");
  explicit vkResourceLocator(const vkResourceLocator &resource, const vkString &resourceName);

  const vkString &GetResourceFile() const;
  const vkString &GetResourceName() const;

  bool operator< (const vkResourceLocator &o) const;
  bool operator== (const vkResourceLocator &o) const;

  bool IsValid() const;

private:
  vkString m_resourceFile;
  vkString m_resourceName;

};
