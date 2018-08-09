#pragma once

#include <list>
#include <Valkyrie/Types.hh>


struct IFile;

namespace assetmanager
{

class AssetWriter
{
public:
  AssetWriter();
  ~AssetWriter();

  void AddEntry(const vkString &name, const vkString &typeID, vkUInt32 length, const vkUInt8* buffer);

  void Output(IFile *file);

private:

  struct Entry
  {
    char typeID[64];
    char name[64];
    vkUInt32 offset;

    vkUInt32 length;
    vkUInt8 *buffer;
  };

  std::list<Entry> m_entries;
};

}
