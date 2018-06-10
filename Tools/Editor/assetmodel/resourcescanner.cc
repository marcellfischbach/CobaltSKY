
#include <assetmodel/resourcescanner.hh>
#include <assetmodel/asset.hh>
#include <cobalt/core/csvfs.hh>
#include <csfile/csffile.hh>
#include <csfile/csfentry.hh>


namespace asset::model
{
  
  ResourceScanner::ResourceScanner(Asset *asset)
    : m_asset(asset)
  {

  }

  void ResourceScanner::Scan()
  {
    std::string path = csVFS::Get()->GetAbsolutePath(m_asset->GetResourceLocator());
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


  void ResourceScanner::Scan(const csfEntry *entry)
  {
    if (entry->HasAttribute("locator"))
    {
      csResourceLocator locator(entry->GetAttribute("locator"));
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