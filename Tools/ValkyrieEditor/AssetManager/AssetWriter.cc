
#include <AssetManager/AssetWriter.hh>
#include <Valkyrie/Core/IFile.hh>
#include <Valkyrie/Defs.hh>


AssetWriter::AssetWriter(IFile *file)
  : m_file(0)
{
  VK_SET(m_file, file);
}

AssetWriter::~AssetWriter()
{
  VK_RELEASE(m_file);
}



void AssetWriter::AddEntry(const vkString& typeID, const vkString &name, const vkString &loaderName, vkUInt32 length, const vkUInt8* buffer)
{
  Entry entry;
  VK_ZERO(entry);
  snprintf(entry.typeID, 8, typeID.c_str());
  snprintf(entry.name, 32, name.c_str());
  snprintf(entry.loaderName, 64, loaderName.c_str());
  entry.length = length;
  entry.buffer = new vkUInt8[length];
  memcpy(entry.buffer, buffer, length);
  m_entries.push_back(entry);
}

void AssetWriter::Output()
{
  const vkUInt32 HEADER_SIZE = 10;
  const vkUInt32 ENTRY_HEADER_SIZE = 44;
  const vkUInt32 ENTRY_SIZE = 68;

  vkUInt32 offset = HEADER_SIZE + m_entries.size() * ENTRY_HEADER_SIZE;
  for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
  {
    it->offset = offset;
    offset += ENTRY_SIZE + it->length;
  }

  // now the offset contains the full length;

}