
#include <assetmanager/assetmanageraction.hh>

class AssetManagerDeleteAction : public AssetManagerAction
{
public:
	AssetManagerDeleteAction();
	virtual ~AssetManagerDeleteAction();

	virtual bool ShouldShow(AssetManagerWidget *assetManager) const;

	virtual bool IsEnabled(AssetManagerWidget *assetManager) const;

	virtual QString GetMenuEntryName(AssetManagerWidget *assetManager) const;

	virtual bool PerformAction(AssetManagerWidget *assetManager) const;

};