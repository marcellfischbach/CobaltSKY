#pragma once


#include <valkyrie/core/vkstring.hh>

class AssetDescriptor
{
public:
  AssetDescriptor(const vkString &assetFileName = vkString(""), const vkString &assetType = vkString(""));

  const vkString &GetAssetResourceName() const;
  const vkString &GetAssetFileName() const;
  const vkString &GetAssetType() const;

  bool operator<(const AssetDescriptor &other) const;
  bool operator==(const AssetDescriptor &other) const;
private:
  vkString m_assetResourceName;
  vkString m_assetFileName;
  vkString m_assetType;
};