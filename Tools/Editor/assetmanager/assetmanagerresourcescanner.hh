#pragma once

#include <valkyrie/core/vkstring.hh>
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
  void ScanReference(const vkString &assetName);
  void AddResourceName(const std::string &name);

  const std::set <std::string> &GetAllResources() const
  {
    return m_allResourceNames;
  }

  const std::set<std::pair<std::string, std::string>> &GetDependencies() const
  {
    return m_dependencies;
  }

private:

  void Scan(const vkString &rootPath, const vkString &relPath);
  void ScanReference(const vkString &assetName, const TiXmlElement *element);

  
  std::set<std::string> m_allResourceNames;
  std::set<std::pair<std::string, std::string>> m_dependencies;
};