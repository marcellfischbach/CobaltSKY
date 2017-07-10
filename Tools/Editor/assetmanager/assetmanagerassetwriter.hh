#pragma once

#include <list>
#include <cobalt/cstypes.hh>


struct iFile;


class AssetManagerAssetWriter
{
public:
  AssetManagerAssetWriter();
  ~AssetManagerAssetWriter();

  void AddEntry(const csString &name, const csString &typeID, csUInt32 length, const csUInt8* buffer);

  void Output(iFile *file);

private:

  struct Entry
  {
    char name[64];
    char typeID[64];
    csUInt32 offset;

    csUInt32 length;
    csUInt8 *buffer;
  };

  std::list<Entry> m_entries;
};

