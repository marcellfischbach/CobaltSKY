#pragma once

#include <assetmanager/actions/assetmanagernewaction.hh>

class ShaderGraphEditorNewAction : public AssetManagerNewAction
{
public:
  ShaderGraphEditorNewAction();
  virtual ~ShaderGraphEditorNewAction();

  virtual bool ShouldShow(AssetManagerWidget *assetManager) const;
  virtual bool IsEnabled(AssetManagerWidget *assetManager) const;

  virtual QString GetMenuEntryName(AssetManagerWidget *assetManager) const;

  virtual csResourceLocator CreateNewAsset(AssetManagerWidget *assetManager) const;

};