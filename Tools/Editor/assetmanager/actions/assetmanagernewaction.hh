#pragma once

#include <assetmanager/assetmanageraction.hh>
#include <assetmodel/folder.hh>
#include <cobalt/core/csresourcelocator.hh>

namespace asset::model
{
	class Folder;

}

class AssetManagerNewAction : public AssetManagerAction
{
public:
  virtual ~AssetManagerNewAction();

  virtual bool PerformAction(AssetManagerWidget *assetManager) const;

protected:
	AssetManagerNewAction(const std::string &baseName = std::string(""));
	virtual bool CreateNewAsset(asset::model::Folder *folder, const csResourceLocator &locator, const std::string &fileName) const
	{
		return false;
	}

  virtual csResourceLocator CreateNewAsset(AssetManagerWidget *assetManager) const = 0;

private:
	std::string m_baseName;
};