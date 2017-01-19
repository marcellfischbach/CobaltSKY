
#include <assetdescriptor.hh>


AssetDescriptor::AssetDescriptor(const vkString &assetFileName, const vkString &assetType)
  : m_assetFileName(assetFileName)
  , m_assetType(assetType)
{

}

const vkString &AssetDescriptor::GetAssetFileName() const
{
  return m_assetFileName;
}

const vkString &AssetDescriptor::GetAssetType() const
{
  return m_assetType;
}


bool AssetDescriptor::operator<(const AssetDescriptor &other) const
{
  return m_assetFileName < other.m_assetFileName;
}

bool AssetDescriptor::operator==(const AssetDescriptor &other) const
{
  return m_assetFileName == other.m_assetFileName;
}

