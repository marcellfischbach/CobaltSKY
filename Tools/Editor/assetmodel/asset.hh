
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

		virtual bool IsAsset() const;
		virtual Asset *AsAsset();
		virtual const Asset*AsAsset() const;


    void ClearReferences();
    void AddReference(const csResourceLocator &locator);
    void RenameReference(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);

    const std::set<csResourceLocator> &GetReferences() const;

    virtual csResourceLocator GetResourceLocator() const;
		virtual csResourceLocator GetNamedResourceLocator(const std::string &name) const;

	protected:
		virtual csResourceLocator Construct(const csResourceLocator &parentLocator) const;


  private:
    std::string m_fileName;

    std::string m_assetType;

    std::set<csResourceLocator> m_references;
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

inline const std::set<csResourceLocator> &asset::model::Asset::GetReferences() const
{
  return m_references;
}
