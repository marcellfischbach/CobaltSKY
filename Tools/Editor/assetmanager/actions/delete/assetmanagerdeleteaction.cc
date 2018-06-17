
#include <assetmanager/actions/delete/assetmanagerdeleteaction.hh>
#include <assetmanager/actions/delete/assetmanagerdeletedialog.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/model.hh>
#include <assetmodel/vfsentry.hh>
#include <cobalt/core/csvfs.hh>
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
	return entries.size() == 1;
}

QString AssetManagerDeleteAction::GetMenuEntryName(AssetManagerWidget *assetManager) const
{
	const std::vector<asset::model::Asset*> entries = assetManager->GetSelectedAssets();
	if (entries.empty() || entries.size() > 1)
	{
		return QObject::tr("Delete");
	}
	else
	{
		asset::model::Asset* entry = entries[0];
		return QObject::tr("Delete %1").arg(QString(entry->GetName().c_str()));
	}
}

bool AssetManagerDeleteAction::PerformAction(AssetManagerWidget *assetManager) const
{
  const std::vector<asset::model::Asset*> entries = assetManager->GetSelectedAssets();
  if (entries.empty())
  {
    return false;
  }
  asset::model::Asset* entry = entries[0];


  AssetManagerDeleteDialog dlg(assetManager);
  dlg.SetAssetName(entry->GetName());
  int res = dlg.exec(); 
  if (res != QDialog::Accepted)
  {
    return false;
  }

  bool deleteOverloaded = dlg.IsDeleteOverloaded();
  bool deleteSuper = dlg.IsDeleteSuper();

  const asset::model::VFSEntry *vfsEntry = entry->GetVFSEntry();
  int entityPriority = vfsEntry->GetPriority();

  const std::set<asset::model::Entry*> &otherEntries = entry->GetModel()->GetEntries(entry->GetResourceLocator());

  for (auto deleteEntry : otherEntries)
  {
    const asset::model::VFSEntry *deleteVFSEntry = deleteEntry->GetVFSEntry();

    int deletePriority = deleteVFSEntry->GetPriority();

    if (deletePriority < entityPriority && !deleteSuper)
    {
      continue;
    }
    else if (deletePriority > entityPriority && !deleteOverloaded)
    {
      continue;
    }

    deleteEntry->Delete();

  }


	return true;
}
