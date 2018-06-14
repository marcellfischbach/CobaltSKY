
#include <assetmanager/actions/assetmanagerdeleteaction.hh>
#include <assetmodel/asset.hh>
#include <vector>
#include <QObject>
#include <assetmanager/assetmanagerwidget.hh>

AssetManagerDeleteAction::AssetManagerDeleteAction()
	: AssetManagerAction()
{

}
AssetManagerDeleteAction::~AssetManagerDeleteAction()
{

}

bool AssetManagerDeleteAction::ShouldShow(AssetManagerWidget *assetManager) const
{
	return true;
}

bool AssetManagerDeleteAction::IsEnabled(AssetManagerWidget *assetManager) const
{
	const std::vector<asset::model::Asset*> entries = assetManager->GetSelectedAssets();
	return !entries.empty();
}

QString AssetManagerDeleteAction::GetMenuEntryName(AssetManagerWidget *assetManager) const
{
	const std::vector<asset::model::Asset*> entries = assetManager->GetSelectedAssets();
	if (entries.empty())
	{
		return QObject::tr("Rename");
	}
	asset::model::Asset* entry = entries[0];
	return QObject::tr("Rename %1").arg(QString(entry->GetName().c_str()));
}

bool AssetManagerDeleteAction::PerformAction(AssetManagerWidget *assetManager) const
{
	const std::vector<asset::model::Asset*> entries = assetManager->GetSelectedAssets();
	if (entries.empty())
	{
		return false;
	}
	asset::model::Asset* entry = entries[0];
	entry->Delete();


	return true;
}
