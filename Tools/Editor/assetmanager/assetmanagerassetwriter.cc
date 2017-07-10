
#include <assetmanager/assetmanagerassetwriter.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/csdefs.hh>

  AssetManagerAssetWriter::AssetManagerAssetWriter()
{
}

  AssetManagerAssetWriter::~AssetManagerAssetWriter()
{
}



void AssetManagerAssetWriter::AddEntry(const csString &name, const csString &typeID, csUInt32 length, const csUInt8* buffer)
{
  Entry entry;
  memset(&entry, 0, sizeof(Entry));
  snprintf(entry.name, 64, name.c_str());
  snprintf(entry.typeID, 64, typeID.c_str());
  entry.length = length;
  entry.buffer = new csUInt8[length];
  memcpy(entry.buffer, buffer, length);
  m_entries.push_back(entry);
}

void AssetManagerAssetWriter::Output(iFile *file)
{
  const csUInt32 HEADER_SIZE = 14;
  const csUInt32 ENTRY_HEADER_SIZE = 132;
  const csUInt32 ENTRY_SIZE = 4;

  csUInt32 offset = HEADER_SIZE + m_entries.size() * ENTRY_HEADER_SIZE;
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
  csUInt32 version = CS_VERSION(1, 0, 0);
  file->Write(&version, sizeof(csUInt32));

  // entry count
  csUInt16 numEntries = (csUInt16)m_entries.size();
  file->Write(&numEntries, sizeof(csUInt16));

  for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
  {
    Entry &entry = *it;
    // write the needed informations for the entry header
    file->Write(entry.name, sizeof(entry.name));
    file->Write(entry.typeID, sizeof(entry.typeID));
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
