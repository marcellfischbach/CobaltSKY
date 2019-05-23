
#include <cobalt/core/resource/csassetcsfloader.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csresourcemanager.hh>

cs::AssetCSFLoader::AssetCSFLoader()
  : cs::BaseCSFLoader()
{

}

cs::AssetCSFLoader::~AssetCSFLoader()
{

}

bool cs::AssetCSFLoader::CanLoad(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  if (entry->IsRoot())
  {
    return entry->GetEntry("asset");
  }

  return entry->GetTagName() == std::string ("asset");
}


const cs::Class *cs::AssetCSFLoader::EvalClass(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(userData);
  const csfEntry *assetEntry = entry;
  if (assetEntry && assetEntry->IsRoot())
  {
    assetEntry = assetEntry->GetEntry("asset");
  }
  if (!assetEntry)
  {
    return nullptr;
  }

  const csfEntry *dataEntry = FindEntryByTagName(assetEntry, locator.GetResourceName());
  if (!dataEntry)
  {
    return nullptr;
  }

  const csfEntry *firstChild = dataEntry->GetChild(0);
  if (!firstChild)
  {
    return nullptr;
  }

  return cs::ResourceManager::Get()->EvalClass(firstChild, locator);
}

cs::ResourceWrapper *cs::AssetCSFLoader::Load(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(userData);
  const csfEntry *assetEntry = entry;
  if (assetEntry && assetEntry->IsRoot())
  {
    assetEntry = assetEntry->GetEntry("asset");
  }
  if (!assetEntry)
  {
    return nullptr;
  }

  const csfEntry *dataEntry = FindEntryByTagName(assetEntry, locator.GetResourceName());
  if (!dataEntry)
  {
    return nullptr;
  }

  const csfEntry *firstChild = dataEntry->GetChild(0);
  if (!firstChild)
  {
    return nullptr;
  }

  return cs::ResourceManager::Get()->Load(firstChild, locator);
}
