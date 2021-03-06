 
#pragma once

#include <editor/editorexport.hh>
#include <editor/assetmodel/entry.hh>

namespace asset::model
{
  class Model;
	class Asset;
  class EDITOR_API Folder : public Entry
  {
  public:
    Folder(Model *model, const std::string &folderName);
    virtual ~Folder();

    virtual void Add(Entry *entry, ModelTransaction &tr);

		virtual bool IsFolder() const;
		virtual Folder *AsFolder();
		virtual const Folder *AsFolder() const;

		const std::string GetNewAssetName(const std::string &baseName) const;
		cs::ResourceLocator GetNewResourceLocator(const std::string &baseName) const;
		Asset *CreateNewAsset(const std::string &baseName) const;

    virtual cs::ResourceLocator GetResourceLocator() const;
  protected:
    Folder(Model *mode, Entry::Type type);


  };
}

