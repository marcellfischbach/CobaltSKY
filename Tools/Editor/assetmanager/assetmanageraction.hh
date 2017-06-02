#pragma once


class QString;
class AssetManagerWidget;

struct AssetManagerAction
{
  AssetManagerAction() { }
  virtual ~AssetManagerAction() { }

  virtual bool ShouldShow (AssetManagerWidget *assetManager) const = 0;
  virtual bool IsEnabled(AssetManagerWidget *assetManager) const = 0;

  virtual QString GetMenuEntryName(AssetManagerWidget *assetManager) const = 0;

  virtual bool PerformAction(AssetManagerWidget *assetManager) const = 0;
};