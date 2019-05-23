
#pragma once

#include <editor/editorexport.hh>

class cs::file::Entry;

namespace asset::model
{
  class Asset;
  class AssetScanner
  {
  public:
    AssetScanner(asset::model::Asset *asset);
    void Scan();

  private:
    void Scan(const cs::file::Entry *entry);
    asset::model::Asset * m_asset;
  };

}