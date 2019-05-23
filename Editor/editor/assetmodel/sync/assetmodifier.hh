
#pragma once

#include <editor/editorexport.hh>

namespace cs
{
class ResourceLocator;
}

class csfEntry;

namespace asset::model
{
  class Asset;
  class AssetModifier
  {
  public:
    AssetModifier(asset::model::Asset *asset);

    void ReplaceLocator(const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator);
    void ClearLocator(const cs::ResourceLocator &locator);

  private:
    void ReplaceLocator(csfEntry *entry, const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator);
    asset::model::Asset *m_asset;
  };
}