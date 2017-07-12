#pragma once

#include <assetmanager/assetmanageraction.hh>
#include <cobalt/core/csresourcelocator.hh>

class AssetManagerNewAction : public AssetManagerAction
{
public:
  virtual ~AssetManagerNewAction();

  virtual bool PerformAction(AssetManagerWidget *assetManager) const;

protected:
  AssetManagerNewAction();
  virtual csResourceLocator CreateNewAsset(AssetManagerWidget *assetManager) const = 0;

};