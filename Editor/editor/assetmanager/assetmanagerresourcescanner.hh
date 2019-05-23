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
    cs::ResourceLocator locator;
    std::string name;
    std::string typeName;
    unsigned priority;
    std::vector<cs::ResourceLocator> references;
  };

  AssetManagerResourceScanner();

  void ScanPath();
  void ScanReference(const cs::ResourceLocator &locator);
  void ScanReference(Entry &entry);
  void AddEntry(const cs::ResourceLocator &locator, const std::string &name, const std::string &typeName);

  const std::vector<Entry> &GetAllEntries() const;


private:

  void ScanPath(const cs::VFS::Entry &entry, const std::string &relPath);
  void ScanReference(Entry &assetEntry, const csfEntry *entry);
  void EvalTypeName(Entry &assetEntry, const csfEntry *rootEntry);

  
  std::vector<Entry> m_entries;
};
