
#include <assetmanager/actions/rename/assetmanagerrenameaction.hh>
#include <assetmanager/actions/rename/assetmanagerrenamedialog.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/model.hh>
#include <assetmodel/modeltransaction.hh>
#include <assetmodel/vfsentry.hh>
#include <cobalt/core/csvfs.hh>
#include <vector>
#include <QObject>
#include <assetmanager/assetmanagerwidget.hh>

AssetManagerRenameAction::AssetManagerRenameAction()
  : AssetManagerAction()
{

}

AssetManagerRenameAction::~AssetManagerRenameAction()
{

}

bool AssetManagerRenameAction::ShouldShow(AssetManagerWidget *assetManager) const
{
  return true;
}

bool AssetManagerRenameAction::IsEnabled(AssetManagerWidget *assetManager) const
{
	const std::vector<asset::model::Asset*> entries = assetManager->GetSelectedAssets();
	return entries.size() == 1;

}

QString AssetManagerRenameAction::GetMenuEntryName(AssetManagerWidget *assetManager) const
{
	const std::vector<asset::model::Asset*> entries = assetManager->GetSelectedAssets();
	if (entries.empty() || entries.size () > 1)
	{
		return QObject::tr("Rename");
	}
	else
	{
		asset::model::Asset* entry = entries[0];
		return QObject::tr("Rename %1").arg(QString(entry->GetName().c_str()));
	}
}

bool AssetManagerRenameAction::PerformAction(AssetManagerWidget *assetManager) const
{
	const std::vector<asset::model::Asset*> entries = assetManager->GetSelectedAssets();
	if (entries.empty())
	{
		return false;
	}
	asset::model::Asset* entry = entries[0];


	AssetManagerRenameDialog dlg(assetManager);
	dlg.SetAssetName(entry->GetName());
	int res = dlg.exec();
	if (res != QDialog::Accepted)
	{
		return false;
	}
	std::string newName = dlg.GetAssetName();
	if (newName == entry->GetName())
	{
		printf("Don't rename %s => %s\n", entry->GetName().c_str(), newName.c_str());
		return false;
	}

	bool renameOverloaded = dlg.IsRenameOverloaded();
	bool renameSuper = dlg.IsRenameSuper();
	

	const asset::model::VFSEntry *vfsEntry = entry->GetVFSEntry();
	int entityPriority = vfsEntry->GetPriority();

	const std::set<asset::model::Entry*> &otherEntriesCopy = std::set<asset::model::Entry*>(entry->GetModel()->GetAllEntriesFor(entry->GetResourceLocator()));
	asset::model::ModelTransaction tr;
	try
	{
		for (auto renameEntry : otherEntriesCopy)
		{
			const asset::model::VFSEntry *renameVFSEntry = renameEntry->GetVFSEntry();
			int deletePriority = renameVFSEntry->GetPriority();

			if (deletePriority < entityPriority && !renameSuper)
			{
				continue;
			}
			else if (deletePriority > entityPriority && !renameOverloaded)
			{
				continue;
			}

			renameEntry->Rename(newName, tr);

		}
		tr.Commit();
	}
	catch (const std::exception &e)
	{
		tr.Rollback();
	}


	return true;
}
