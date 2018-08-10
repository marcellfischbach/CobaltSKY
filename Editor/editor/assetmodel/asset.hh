
#pragma once

#include <editor/editorexport.hh>
#include <editor/assetmodel/entry.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csclass.hh>
#include <string>
#include <set>

namespace asset::model
{
  class Model;
  class EDITOR_API Asset : public Entry
  {
  public:
    Asset(Model *model, const std::string &fileName);
    virtual ~Asset();

    void SetAssetType(const std::string &assetType);
    const std::string &GetAssetType() const;

		virtual void Add(Entry *entry);

		virtual void SetName(const std::string &name);
    const std::string &GetAssetName() const;
		virtual const std::string &GetDisplayName() const;
		virtual const std::string FakeName(const std::string &name) const;

		virtual bool IsAsset() const;
		virtual Asset *AsAsset();
		virtual const Asset*AsAsset() const;

    const csClass *GetClass() const;


    virtual csResourceLocator GetResourceLocator() const;

		void ClearReferences();
		void AddReference(const csResourceLocator &reference);


  private:
    std::string m_assetName;

    std::string m_assetType;

    mutable const csClass *m_cls;
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

inline const std::string &asset::model::Asset::GetAssetName() const
{
  return m_assetName;
}

