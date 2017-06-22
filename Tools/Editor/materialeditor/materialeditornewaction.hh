#pragma once

#include <assetmanager/actions/assetmanagernewaction.hh>

class MaterialEditorNewAction : public AssetManagerNewAction
{
public:
  MaterialEditorNewAction();
  virtual ~MaterialEditorNewAction();

  virtual bool ShouldShow(AssetManagerWidget *assetManager) const;
  virtual bool IsEnabled(AssetManagerWidget *assetManager) const;

  virtual QString GetMenuEntryName(AssetManagerWidget *assetManager) const;

  virtual bool PerformAction(AssetManagerWidget *assetManager) const;

};