
#include <editor/assetmanager/actions/delete/assetmanagerdeleteaction.hh>
#include <editor/assetmanager/actions/delete/assetmanagerdeletedialog.hh>
#include <editor/assetmanager/contextmenu/contextmenucategory.hh>
#include <editor/assetmanager/contextmenu/contextmenuentry.hh>
#include <editor/assetmanager/assetmanagerwidget.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/assetmodel/model.hh>
#include <editor/assetmodel/modeltransaction.hh>
#include <editor/assetmodel/vfsentry.hh>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>
#include <cobalt/core/csvfs.hh>
#include <vector>
#include <QObject>

namespace asset::actions
{


  void AssetManagerDeleteContextMenu::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
  {
    if (entry->IsRoot() || entry->IsVFSEntry())
    {
      return;
    }

    std::string name = "Delete '" + entry->GetDisplayName() + "'";
    builder.AddAction(asset::contextmenu::Category::Edit(), name)
      ->SetAction(new AssetManagerDeleteAction(entry));
  }



  AssetManagerDeleteAction::AssetManagerDeleteAction(asset::model::Entry *entry)
    : asset::actions::ActionCallback()
    , m_entry(entry)
  {

  }

  AssetManagerDeleteAction::~AssetManagerDeleteAction()
  {

  }

  void AssetManagerDeleteAction::Callback()
  {
    AssetManagerDeleteDialog dlg(Editor::Get()->GetMainWindow());
    dlg.SetAssetName(m_entry->GetDisplayName());
    int res = dlg.exec();

    if (res != QDialog::Accepted)
    {
      return;
    }

    bool deleteOverloaded = dlg.IsDeleteOverloaded();
    bool deleteSuper = dlg.IsDeleteSuper();


    int entityPriority = m_entry->GetVFSEntryPriority();

    const std::set<asset::model::Entry*> &otherEntriesCopy = std::set<asset::model::Entry*>(m_entry->GetModel()->GetAllEntriesFor(m_entry->GetResourceLocator()));
    asset::model::ModelTransaction tr;
    try
    {
      for (auto deleteEntry : otherEntriesCopy)
      {
        const asset::model::VFSEntry *renameVFSEntry = deleteEntry->GetVFSEntry();
        int deletePriority = renameVFSEntry->GetPriority();

        if (deletePriority < entityPriority && !deleteSuper)
        {
          continue;
        }
        else if (deletePriority > entityPriority && !deleteOverloaded)
        {
          continue;
        }

        deleteEntry->Delete(tr);

      }
      tr.Commit();
    }
    catch (const std::exception &e)
    {
      tr.Rollback();
    }
  }


}




















#if 0
#include <assetmanager/actions/delete/assetmanagerdeleteaction.hh>
#include <assetmanager/actions/delete/assetmanagerdeletedialog.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/model.hh>
#include <assetmodel/modeltransaction.hh>
#include <assetmodel/vfsentry.hh>
#include <cobalt/core/csvfs.hh>
#include <vector>
#include <QObject>
#include <assetmanager/assetmanagerwidget.hh>
#include <iostream>

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

  std::string entryName = entry->GetName();
  AssetManagerDeleteDialog dlg(assetManager);
  dlg.SetAssetName(entryName);
  int res = dlg.exec(); 
  if (res != QDialog::Accepted)
  {
    return false;
  }

  bool deleteOverloaded = dlg.IsDeleteOverloaded();
  bool deleteSuper = dlg.IsDeleteSuper();

  int entityPriority = entry->GetVFSEntryPriority();

  const std::set<asset::model::Entry*> &otherEntries = entry->GetModel()->GetAllEntriesFor(entry->GetResourceLocator());

	asset::model::ModelTransaction tr;
	bool commit = true;
	try
	{
		for (auto deleteEntry : otherEntries)
		{
			int deletePriority = deleteEntry->GetVFSEntryPriority();

			if (deletePriority < entityPriority && !deleteSuper)
			{
				continue;
			}
			else if (deletePriority > entityPriority && !deleteOverloaded)
			{
				continue;
			}

			deleteEntry->Delete(tr);

		}
		tr.Commit();
	}
	catch (const std::exception &e)
	{
    std::cout << "Unable to delete " << entryName << ": " << e.what() << std::endl;
		tr.Rollback();
	}


	return true;
}

#endif