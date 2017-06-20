#pragma once

#include <cobalt/core/csstring.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csvfs.hh>
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
  void ScanReference(const csResourceLocator &locator);
  void AddResource(const csResourceLocator &locator);

  const std::set <csResourceLocator> &GetAllResources() const
  {
    return m_allResourceLocators;
  }

  const std::set<std::pair<csResourceLocator, csResourceLocator>> &GetReferences() const
  {
    return m_references;
  }

private:

  void Scan(const csVFS::Entry &entry, const csString &relPath);
  void ScanReference(const csResourceLocator &assetName, const TiXmlElement *element);

  
  std::set<csResourceLocator> m_allResourceLocators;
  std::set<std::pair<csResourceLocator, csResourceLocator>> m_references;
};