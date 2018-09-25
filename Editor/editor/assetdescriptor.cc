
#include <editor/assetdescriptor.hh>
#include <editor/editor.hh>


AssetDescriptor::AssetDescriptor(const csResourceLocator &locator, const std::string &assetType)
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

const std::string &AssetDescriptor::GetAssetType() const
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

