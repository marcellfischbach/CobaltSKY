
#include <assetmanager/assetmanageraction.hh>

class AssetManagerRenameAction : public AssetManagerAction 
{
public:
  AssetManagerRenameAction();
  virtual ~AssetManagerRenameAction();

  virtual bool ShouldShow(AssetManagerWidget *assetManager) const;

  virtual bool IsEnabled(AssetManagerWidget *assetManager) const;

  virtual QString GetMenuEntryName(AssetManagerWidget *assetManager) const;

  virtual bool PerformAction(AssetManagerWidget *assetManager) const;

};