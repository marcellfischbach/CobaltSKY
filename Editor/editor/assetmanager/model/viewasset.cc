

#include <editor/assetmanager/model/viewasset.hh>
#include <editor/assetmodel/asset.hh>

namespace asset::model
{

  ViewAsset::ViewAsset(Asset *asset)
    : ViewEntry(asset)
    , m_asset(asset)
  {

  }

  ViewAsset::~ViewAsset()
  {

  }

  void ViewAsset::Add(ViewEntry *entry)
  {
    // no children at asset level
    return;
  }

	void ViewAsset::Remove(ViewEntry *entry)
	{
		// no children at asset level
		return;
	}

  void ViewAsset::Sort()
  {
    return;
  }

  const std::string ViewAsset::GetText() const
  {
    if (m_asset)
    {
      return m_asset->GetAssetName () + "(" + m_asset->GetAssetType() + ")";
    }
    return ViewEntry::GetText();
  }
}