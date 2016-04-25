#pragma once

#include <list>
#include <Valkyrie/Types.hh>


struct IFile;

class AssetWriter
{
public:
  AssetWriter();
  ~AssetWriter();

  void AddEntry(const vkString& typeID, const vkString &name, const vkString &loaderName, vkUInt32 length, const vkUInt8* buffer);

  void Output(IFile *file);

private:
  IFile *m_file;

  struct Entry
  {
    char typeID[8];
    char name[64];
    vkUInt32 offset;

    char loaderName[256];
    vkUInt32 length;
    vkUInt8 *buffer;
  };

  std::list<Entry> m_entries;
};