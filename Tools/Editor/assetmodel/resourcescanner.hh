
#pragma once


class csfEntry;

namespace asset::model
{
  class Asset;
  class ResourceScanner
  {
  public:
    ResourceScanner(Asset *asset);
    void Scan();

  private:
    void Scan(const csfEntry *entry);
    Asset * m_asset;
  };

}