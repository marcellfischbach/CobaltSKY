
#include <assetdescriptor.hh>
#include <editor.hh>


AssetDescriptor::AssetDescriptor(const vkResourceLocator &locator, const vkString &assetType)
  : m_locator(locator)
  , m_assetType(assetType)
{
}

const vkResourceLocator &AssetDescriptor::GetLocator() const
{
  return m_locator;
}

const vkString &AssetDescriptor::GetAssetType() const
{
  return m_assetType;
}


bool AssetDescriptor::operator<(const AssetDescriptor &other) const
{
  return m_locator < other.m_locator;
}

bool AssetDescriptor::operator==(const AssetDescriptor &other) const
{
  return m_locator == other.m_locator;
}

