
#pragma once

#include <assetmodel/entry.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <string>
#include <set>

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

		virtual void Add(Entry *entry);

		virtual void SetName(const std::string &name);
		virtual const std::string FakeName(const std::string &name) const;

		virtual bool IsAsset() const;
		virtual Asset *AsAsset();
		virtual const Asset*AsAsset() const;


    virtual csResourceLocator GetResourceLocator() const;

		void ClearReferences();
		void AddReference(const csResourceLocator &reference);


  private:
    std::string m_assetName;

    std::string m_assetType;

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
