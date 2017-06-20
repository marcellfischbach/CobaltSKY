#pragma once


#include <cobalt/core/csstring.hh>
#include <cobalt/core/csresourcelocator.hh>

class AssetDescriptor
{
public:
  AssetDescriptor(const csResourceLocator &locator = csResourceLocator(), const csString &assetType = csString(""));

  const csResourceLocator &GetLocator() const;
  const csString &GetAssetType() const;

  bool operator<(const AssetDescriptor &other) const;
  bool operator==(const AssetDescriptor &other) const;
private:
  csResourceLocator m_locator;
  csString m_assetType;
};