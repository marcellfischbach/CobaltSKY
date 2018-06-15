
#pragma once

#include <assetmodel/entry.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <string>
#include <vector>

namespace asset::model
{
  class Model;
  class Asset : public Entry
  {
  public:
    Asset(Model *model, const std::string &fileName);
    virtual ~Asset();

    void SetAssetType(const std::string &assetType);
    const std::string &GetAssetType() const;

		virtual void SetName(const std::string &name);

		virtual bool IsAsset() const;
		virtual Asset *AsAsset();
		virtual const Asset*AsAsset() const;


    void ClearReferences();
    void AddReference(const csResourceLocator &locator);

    const std::vector<csResourceLocator> &GetReferences() const;

    virtual csResourceLocator GetResourceLocator() const;

  private:
    std::string m_fileName;

    std::string m_assetType;

    std::vector<csResourceLocator> m_references;
  };


 

}



inline void asset::model::Asset::SetAssetType(const std::string &assetType)
{
  m_assetType = assetType;
}

inline const std::string &asset::model::Asset::GetAssetType() const
{
  return m_assetType;
}

inline const std::vector<csResourceLocator> &asset::model::Asset::GetReferences() const
{
  return m_references;
}
