
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


  void AssetModifier::ClearLocator(const cs::ResourceLocator &locator)
  {
    ReplaceLocator(locator, cs::ResourceLocator());
  }

  void AssetModifier::ReplaceLocator(const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator)
  {
    const cs::ResourceLocator &locator = m_asset->GetResourceLocator();
    std::string fullPath = cs::VFS::Get()->GetAbsolutePath(locator);
    csfFile file;
    if (file.Parse(fullPath, false))
    {
      ReplaceLocator(file.GetRoot(), oldLocator, newLocator);
      cs::file::Writer::Write(file, locator);
    }
  }

  void AssetModifier::ReplaceLocator(csfEntry *entry, const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator)
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