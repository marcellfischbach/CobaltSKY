#pragma once

#include <list>
#include <Valkyrie/Types.hh>


struct IFile;

class AssetWriter
{
public:
  AssetWriter(IFile *file);
  ~AssetWriter();

  void AddEntry(const vkString& typeID, const vkString &name, const vkString &loaderName, vkUInt32 length, const vkUInt8* buffer);

  void Output();

private:
  IFile *m_file;

#pragma pack(1)
  struct Entry
  {
    char typeID[8];
    char name[32];
    vkUInt32 offset;

    char loaderName[64];
    vkUInt32 length;
    vkUInt8 *buffer;
  };

  std::list<Entry> m_entries;
};