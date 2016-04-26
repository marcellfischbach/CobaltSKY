#pragma once

#include <list>
#include <Valkyrie/Types.hh>


struct IFile;

class AssetWriter
{
public:
  AssetWriter();
  ~AssetWriter();

  void AddEntry(const vkString &typeID, const vkString &name, vkUInt32 length, const vkUInt8* buffer);

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