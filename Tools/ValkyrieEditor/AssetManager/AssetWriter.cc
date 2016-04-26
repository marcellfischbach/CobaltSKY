
#include <AssetManager/AssetWriter.hh>
#include <Valkyrie/Core/IFile.hh>
#include <Valkyrie/Defs.hh>


AssetWriter::AssetWriter()
{
}

AssetWriter::~AssetWriter()
{
}



void AssetWriter::AddEntry(const vkString &typeID, const vkString &name, vkUInt32 length, const vkUInt8* buffer)
{
  Entry entry;
  VK_ZERO(entry);
  snprintf(entry.typeID, 64, typeID.c_str());
  snprintf(entry.name, 64, name.c_str());
  entry.length = length;
  entry.buffer = new vkUInt8[length];
  memcpy(entry.buffer, buffer, length);
  m_entries.push_back(entry);
}

void AssetWriter::Output(IFile *file)
{
  const vkUInt32 HEADER_SIZE = 14;
  const vkUInt32 ENTRY_HEADER_SIZE = 132;
  const vkUInt32 ENTRY_SIZE = 4;

  vkUInt32 offset = HEADER_SIZE + m_entries.size() * ENTRY_HEADER_SIZE;
  for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
  {
    it->offset = offset;
    offset += ENTRY_SIZE + it->length;
  }

  // now the offset contains the full length;

  //
  // write the header of the asset file

  // magic number
  file->Write("VALASSET", sizeof(char) * 8);

  // current version 1.0.0
  vkUInt32 version = VK_VERSION(1, 0, 0);
  file->Write(&version, sizeof(vkUInt32));

  // entry count
  vkUInt16 numEntries = (vkUInt16)m_entries.size();
  file->Write(&numEntries, sizeof(vkUInt16));

  for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
  {
    Entry &entry = *it;
    // write the needed informations for the entry header
    file->Write(entry.typeID, sizeof(entry.typeID));
    file->Write(entry.name, sizeof(entry.name));
    file->Write(&entry.offset, sizeof(entry.offset));
  }

  for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
  {
    Entry &entry = *it;

    // write the information needed for loading the entry
    file->Write(&entry.length, sizeof(entry.length));
    file->Write(entry.buffer, entry.length);
  }
}