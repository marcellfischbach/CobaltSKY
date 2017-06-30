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
  struct Entry
  {
    csResourceLocator locator;
    std::string name;
    std::string typeName;
    std::vector<csResourceLocator> references;
  };

  AssetManagerResourceScanner();

  void ScanPath();
  void ScanReference(const csResourceLocator &locator);
  void ScanReference(Entry &entry);
  void AddEntry(const csResourceLocator &locator, const std::string &name, const std::string &typeName);

  const std::vector<Entry> GetAllEntries() const;


private:

  void ScanPath(const csVFS::Entry &entry, const csString &relPath);
  void ScanReference(Entry &entry, const TiXmlElement *element);
  void EvalTypeName(Entry &entry, const TiXmlElement *rootElement);

  
  std::map<csResourceLocator, Entry> m_entries;
};