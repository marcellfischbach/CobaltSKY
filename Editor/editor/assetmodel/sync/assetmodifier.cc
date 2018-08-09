
#include <editor/assetmodel/sync/assetmodifier.hh>
#include <editor/assetmodel/asset.hh>
#include <cobalt/core/csfwriter.hh>
#include <cobalt/core/csvfs.hh>
#include <csfile/csffile.hh>


namespace asset::model
{
  AssetModifier::AssetModifier(asset::model::Asset *asset)
    : m_asset(asset)
  {

  }


  void AssetModifier::ClearLocator(const csResourceLocator &locator)
  {
    ReplaceLocator(locator, csResourceLocator());
  }

  void AssetModifier::ReplaceLocator(const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
  {
    const csResourceLocator &locator = m_asset->GetResourceLocator();
    std::string fullPath = csVFS::Get()->GetAbsolutePath(locator);
    csfFile file;
    if (file.Parse(fullPath, false))
    {
      ReplaceLocator(file.GetRoot(), oldLocator, newLocator);
      csfWriter::Write(file, locator);
    }
  }

  void AssetModifier::ReplaceLocator(csfEntry *entry, const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
  {
    if (entry->HasAttribute("locator"))
    {
      if (entry->GetAttribute("locator") == oldLocator.Encode())
      {
        size_t idx = entry->GetAttributeIndex("locator");
        entry->RemoveAttribute(idx);
        entry->AddAttribute("locator", newLocator.Encode());
      }
    }

    for (size_t i = 0, in = entry->GetNumberOfChildren(); i < in; ++i)
    {
      ReplaceLocator(entry->GetChild(i), oldLocator, newLocator);
    }
  }
}