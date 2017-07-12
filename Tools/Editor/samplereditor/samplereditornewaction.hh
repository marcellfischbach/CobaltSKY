#pragma once

#include <assetmanager/actions/assetmanagernewaction.hh>

class SamplerEditorNewAction : public AssetManagerNewAction
{
public:
  SamplerEditorNewAction();
  virtual ~SamplerEditorNewAction();

  virtual bool ShouldShow(AssetManagerWidget *assetManager) const;
  virtual bool IsEnabled(AssetManagerWidget *assetManager) const;

  virtual QString GetMenuEntryName(AssetManagerWidget *assetManager) const;

  virtual csResourceLocator CreateNewAsset(AssetManagerWidget *assetManager) const;

};