#pragma once

#include <valkyrie/core/vkstring.hh>
#include <valkyrie/core/vkresourcelocator.hh>
#include <valkyrie/core/vkvfs.hh>
#include <string>
#include <vector>
#include <map>
#include <set>

class TiXmlElement;
class AssetManagerResourceScanner
{
public:
  AssetManagerResourceScanner();

  void Scan();
  void ScanReference(const vkResourceLocator &locator);
  void AddResource(const vkResourceLocator &locator);

  const std::set <vkResourceLocator> &GetAllResources() const
  {
    return m_allResourceLocators;
  }

  const std::set<std::pair<vkResourceLocator, vkResourceLocator>> &GetReferences() const
  {
    return m_references;
  }

private:

  void Scan(const vkVFS::Entry &entry, const vkString &relPath);
  void ScanReference(const vkResourceLocator &assetName, const TiXmlElement *element);

  
  std::set<vkResourceLocator> m_allResourceLocators;
  std::set<std::pair<vkResourceLocator, vkResourceLocator>> m_references;
};