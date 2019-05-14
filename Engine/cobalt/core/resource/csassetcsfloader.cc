
#include <cobalt/core/resource/csassetcsfloader.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csresourcemanager.hh>

csAssetCSFLoader::csAssetCSFLoader()
  : csBaseCSFLoader()
{

}

csAssetCSFLoader::~csAssetCSFLoader()
{

}

bool csAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  if (entry->IsRoot())
  {
    return entry->GetEntry("asset");
  }

  return entry->GetTagName() == std::string ("asset");
}


const cs::Class *csAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
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

  return csResourceManager::Get()->EvalClass(firstChild, locator);
}

csResourceWrapper *csAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
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

  return csResourceManager::Get()->Load(firstChild, locator);
}
