
#include <editor/assetmodel/sync/assetscanner.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/assetmodel/model.hh>
#include <cobalt/core/csvfs.hh>
#include <csfile/csffile.hh>
#include <csfile/csfentry.hh>


namespace asset::model
{
  
  AssetScanner::AssetScanner(asset::model::Asset *asset)
    : m_asset(asset)
  {

  }

  void AssetScanner::Scan()
  {
    std::string path = cs::VFS::Get()->GetAbsolutePath(m_asset->GetResourceLocator());
    if (path.empty())
    {
      return;
    }

    csfFile file;
    if (!file.Parse(path, false))
    {
      return;
    }

    const csfEntry *dataEntry = file.GetEntry("asset.data");
    if (dataEntry)
    {
      const csfEntry *typeEntry = dataEntry->GetEntry(0);
      if (typeEntry)
      {
        m_asset->SetAssetType(typeEntry->GetTagName());
      }
    }

    m_asset->ClearReferences();
    Scan(file.GetRoot());
  }


  void AssetScanner::Scan(const csfEntry *entry)
  {
    if (entry->HasAttribute("locator"))
    {
      cs::ResourceLocator locator(entry->GetAttribute("locator"));
      if (locator.IsValid())
      {
        m_asset->AddReference(locator);
      }
    }

    for (csSize i=0, in=entry->GetNumberOfChildren(); i<in; ++i) 
    {
      Scan(entry->GetChild(i));
    }
  }
}