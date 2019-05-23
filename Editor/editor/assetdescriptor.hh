#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <string>

class AssetDescriptor
{
public:
  AssetDescriptor(const cs::ResourceLocator &locator = cs::ResourceLocator(), const std::string &assetType = std::string(""));

  bool Renamed(const cs::ResourceLocator &from, const cs::ResourceLocator &to);

  const cs::ResourceLocator &GetLocator() const;
  const std::string &GetAssetType() const;

  bool operator<(const AssetDescriptor &other) const;
  bool operator==(const AssetDescriptor &other) const;
private:
  cs::ResourceLocator m_locator;
  std::string m_assetType;
};