
#pragma once

class csResourceLocator;
class csfEntry;

namespace asset::model
{
  class Asset;
  class AssetModifier
  {
  public:
    AssetModifier(asset::model::Asset *asset);

    void ReplaceLocator(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
    void ClearLocator(const csResourceLocator &locator);

  private:
    void ReplaceLocator(csfEntry *entry, const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
    asset::model::Asset *m_asset;
  };
}