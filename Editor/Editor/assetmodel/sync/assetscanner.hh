
#pragma once


class csfEntry;

namespace asset::model
{
  class Asset;
  class AssetScanner
  {
  public:
    AssetScanner(asset::model::Asset *asset);
    void Scan();

  private:
    void Scan(const csfEntry *entry);
    asset::model::Asset * m_asset;
  };

}