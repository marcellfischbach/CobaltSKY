
#pragma once

#include <editor/editorexport.hh>
#include <editor/assetmanager/model/viewentry.hh>

namespace asset::model
{

  class Asset;

  class ViewAsset : public ViewEntry
  {
  public:
    ViewAsset(Asset* asset);
    virtual ~ViewAsset();

    const Asset *GetAsset() const;
    Asset *GetAsset();

    virtual void Add(ViewEntry *entry);
    virtual void Remove(ViewEntry *entry);
    virtual void Sort();
    virtual const std::string GetText() const;
  private:
    Asset * m_asset;
  };
}

inline const asset::model::Asset *asset::model::ViewAsset::GetAsset() const
{
  return m_asset;
}

inline asset::model::Asset *asset::model::ViewAsset::GetAsset() 
{
  return m_asset;
}
