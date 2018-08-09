#pragma once

#include <editor/editorexport.hh>
#include <list>
#include <cobalt/cstypes.hh>


struct iFile;


class AssetManagerAssetWriter
{
public:
  AssetManagerAssetWriter();
  ~AssetManagerAssetWriter();

  void AddEntry(const std::string &name, const std::string &typeID, csUInt32 length, const csUInt8* buffer);
  void RemoveEntry(const std::string &name);

  bool Import(iFile *file);
  void Output(iFile *file);

private:

  struct Entry
  {
    Entry();
    Entry(const Entry &other);
    ~Entry();
    char name[64];
    char typeID[64];
    csUInt32 offset;

    csUInt32 length;
    csUInt8 *buffer;
  };

  std::list<Entry> m_entries;
};

