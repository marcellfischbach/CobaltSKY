 
#pragma once

#include <assetmodel/entry.hh>

namespace asset::model
{
  class Model;
	class Asset;
  class Folder : public Entry
  {
  public:
    Folder(Model *model, const std::string &folderName);
    virtual ~Folder();

    virtual void Add(Entry *entry);

		virtual bool IsFolder() const;
		virtual Folder *AsFolder();
		virtual const Folder *AsFolder() const;

		const std::string GetNewAssetName(const std::string &baseName) const;
		csResourceLocator GetNewResourceLocator(const std::string &baseName) const;
		Asset *CreateNewAsset(const std::string &baseName) const;

    virtual csResourceLocator GetResourceLocator() const;
		virtual csResourceLocator GetNamedResourceLocator(const std::string &newName) const;
  protected:
    Folder(Model *mode, Entry::Type type);
		virtual csResourceLocator Construct(const csResourceLocator &parentLocator) const;


  };
}

