
#include <assetmanager/actions/rename/assetmanagerrenameaction.hh>
#include <assetmanager/actions/rename/assetmanagerrenamedialog.hh>
#include <assetmanager/contextmenu/contextmenucategory.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/model.hh>
#include <assetmodel/modeltransaction.hh>
#include <assetmodel/vfsentry.hh>
#include <editor.hh>
#include <mainwindow.hh>
#include <cobalt/core/csvfs.hh>
#include <vector>
#include <QObject>
#include <assetmanager/assetmanagerwidget.hh>

namespace asset::actions
{


	void AssetManagerRenameContextMenu::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
	{
		if (entry->IsRoot() || entry->IsVFSEntry())
		{
			return;
		}

		std::string name = "Rename '" + entry->GetDisplayName() + "'";
		builder.AddAction(asset::contextmenu::Category::Edit(), name)
			->SetAction(new AssetManagerRenameAction(entry));
	}



	AssetManagerRenameAction::AssetManagerRenameAction(asset::model::Entry *entry)
		: asset::actions::ActionCallback()
		, m_entry(entry)
	{

	}

	AssetManagerRenameAction::~AssetManagerRenameAction()
	{

	}

	void AssetManagerRenameAction::Callback()
	{
		AssetManagerRenameDialog dlg(Editor::Get()->GetMainWindow());
		dlg.SetAssetName(m_entry->GetDisplayName());
		int res = dlg.exec();

		if (res != QDialog::Accepted)
		{
			return;
		}

	
		std::string newName = dlg.GetAssetName();
		if (newName == m_entry->GetDisplayName())
		{
			printf("Don't rename %s => %s\n", m_entry->GetDisplayName().c_str(), newName.c_str());
			return;
		}

		bool renameOverloaded = dlg.IsRenameOverloaded();
		bool renameSuper = dlg.IsRenameSuper();


		int entityPriority = m_entry->GetVFSEntryPriority();

		const std::set<asset::model::Entry*> &otherEntriesCopy = std::set<asset::model::Entry*>(m_entry->GetModel()->GetAllEntriesFor(m_entry->GetResourceLocator()));
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
	}


}

