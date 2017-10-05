
#include <cobalt/core/resource/csassetcsfloader.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <tixml/tinyxml.h>

csAssetCSFLoader::csAssetCSFLoader()
  : csBaseCSFLoader()
{

}

csAssetCSFLoader::~csAssetCSFLoader()
{

}

bool csAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (entry->IsRoot())
  {
    return entry->GetEntry("asset");
  }

  return entry->GetTagName() == std::string ("asset");
}


const csClass *csAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  const csfEntry *assetEntry = entry;
  if (assetEntry && assetEntry->IsRoot())
  {
    assetEntry = assetEntry->GetEntry("asset");
  }
  if (!assetEntry)
  {
    return 0;
  }

  const csfEntry *dataEntry = FindEntryByTagName(assetEntry, locator.GetResourceName());
  if (!dataEntry)
  {
    return 0;
  }

  const csfEntry *firstChild = dataEntry->GetChild(0);
  if (!firstChild)
  {
    return 0;
  }

  return csResourceManager::Get()->EvalClass(firstChild, locator);
}

iObject *csAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  const csfEntry *assetEntry = entry;
  if (assetEntry && assetEntry->IsRoot())
  {
    assetEntry = assetEntry->GetEntry("asset");
  }
  if (!assetEntry)
  {
    return 0;
  }

  const csfEntry *dataEntry = FindEntryByTagName(assetEntry, locator.GetResourceName());
  if (!dataEntry)
  {
    return 0;
  }

  const csfEntry *firstChild = dataEntry->GetChild(0);
  if (!firstChild)
  {
    return 0;
  }

  return csResourceManager::Get()->Load(firstChild, locator);
}