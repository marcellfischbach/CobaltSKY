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

private:

  void Scan(const vkString &rootPath, const vkString &relPath);
  void ScanReference(const vkString &assetName);
  void ScanReference(const vkString &assetName, const TiXmlElement *element);

  
  std::set<std::string> m_allResourceNames;

  std::map<std::string, std::set<std::string>> m_resourceReferences;
};