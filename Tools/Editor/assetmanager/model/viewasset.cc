

#include <assetmanager/model/viewasset.hh>
#include <assetmodel/asset.hh>

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

  void ViewAsset::Sort()
  {
    return;
  }
}