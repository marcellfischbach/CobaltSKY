#pragma once


#include <valkyrie/core/vkstring.hh>
#include <valkyrie/core/vkresourcelocator.hh>

class AssetDescriptor
{
public:
  AssetDescriptor(const vkResourceLocator &locator = vkResourceLocator(), const vkString &assetType = vkString(""));

  const vkResourceLocator &GetLocator() const;
  const vkString &GetAssetType() const;

  bool operator<(const AssetDescriptor &other) const;
  bool operator==(const AssetDescriptor &other) const;
private:
  vkResourceLocator m_locator;
  vkString m_assetType;
};