#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <string>

class AssetDescriptor
{
public:
  AssetDescriptor(const csResourceLocator &locator = csResourceLocator(), const std::string &assetType = std::string(""));

  bool Renamed(const csResourceLocator &from, const csResourceLocator &to);

  const csResourceLocator &GetLocator() const;
  const std::string &GetAssetType() const;

  bool operator<(const AssetDescriptor &other) const;
  bool operator==(const AssetDescriptor &other) const;
private:
  csResourceLocator m_locator;
  std::string m_assetType;
};