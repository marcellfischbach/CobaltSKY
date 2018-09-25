#pragma once

#include <editor/editorexport.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csvfs.hh>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <string>

class csfEntry;
class AssetManagerResourceScanner
{
public:
  struct Entry
  {
    csResourceLocator locator;
    std::string name;
    std::string typeName;
    unsigned priority;
    std::vector<csResourceLocator> references;
  };

  AssetManagerResourceScanner();

  void ScanPath();
  void ScanReference(const csResourceLocator &locator);
  void ScanReference(Entry &entry);
  void AddEntry(const csResourceLocator &locator, const std::string &name, const std::string &typeName);

  const std::vector<Entry> &GetAllEntries() const;


private:

  void ScanPath(const csVFS::Entry &entry, const std::string &relPath);
  void ScanReference(Entry &assetEntry, const csfEntry *entry);
  void EvalTypeName(Entry &assetEntry, const csfEntry *rootEntry);

  
  std::vector<Entry> m_entries;
};
