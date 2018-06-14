
#include <assetmodel/sync/deletehandler.hh>
#include <assetmodel/model.hh>
#include <assetmodel/entry.hh>
#include <filesystem>
#include <cobalt/core/csvfs.hh>


namespace asset::model::sync
{

	DeleteHandler::DeleteHandler(asset::model::Model *model)
		:QObject()
	{
		connect(model, SIGNAL(EntryAboutToDelete(asset::model::Entry*)), this, SLOT(EntryAboutToDelete(asset::model::Entry*)));
		connect(model, SIGNAL(EntryDeleted(asset::model::Entry*)), this, SLOT(EntryDeleted(asset::model::Entry*)));
	}


	void DeleteHandler::EntryAboutToDelete(asset::model::Entry *entry)
	{
		if (!entry)
		{
			return;
		}
		m_entries[entry] = entry->GetResourceLocator();
	}

	void DeleteHandler::EntryDeleted(asset::model::Entry *entry)
	{
		if (!entry)
		{
			return;
		}
		auto it = m_entries.find(entry);
		if (it == m_entries.end())
		{
			return;
		}

		switch (entry->GetType())
		{
		case asset::model::Entry::eT_Asset:
			DeleteAsset(entry->AsAsset(), it->second);
			break;
		case asset::model::Entry::eT_Folder:
			DeleteFolder(entry->AsFolder(), it->second);
			break;
		case asset::model::Entry::eT_VFSEntry:
		case asset::model::Entry::eT_Root:
			break;
		}

		m_entries.erase(it);

	}
	void DeleteHandler::DeleteAsset(asset::model::Asset *asset, csResourceLocator locator)
	{
		std::string fullPath = csVFS::Get()->GetAbsolutePath(locator);
		if (fullPath.empty())
		{
			return;
		}
		std::filesystem::path path(fullPath);
		std::filesystem::remove(path);
	}

	void DeleteHandler::DeleteFolder(asset::model::Folder *folder, csResourceLocator locator)
	{
		std::string fullPath = csVFS::Get()->GetAbsolutePath(locator);
		if (fullPath.empty())
		{
			return;
		}
		std::filesystem::path path(fullPath);
		std::filesystem::remove_all(path);
	}
}