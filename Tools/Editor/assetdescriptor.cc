
#include <assetdescriptor.hh>
#include <editor.hh>


AssetDescriptor::AssetDescriptor(const csResourceLocator &locator, const csString &assetType)
  : m_locator(locator)
  , m_assetType(assetType)
{
}

bool AssetDescriptor::Renamed(const csResourceLocator &from, const csResourceLocator &to)
{
  if (m_locator == from)
  {
    m_locator = to;
    return true;
  }
  return false;
}

const csResourceLocator &AssetDescriptor::GetLocator() const
{
  return m_locator;
}

const csString &AssetDescriptor::GetAssetType() const
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

