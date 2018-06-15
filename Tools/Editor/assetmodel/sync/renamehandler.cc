
#include <assetmodel/sync/renamehandler.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>
#include <assetmodel/entry.hh>
#include <filesystem>
#include <cobalt/core/csvfs.hh>
#include <csfile/csffile.hh>
#include <csfile/csfentry.hh>
#include <cobalt/core/csfwriter.hh>


namespace asset::model::sync
{

	RenameHandler::RenameHandler(asset::model::Model *model)
		:QObject()
		, m_model(model)
	{
		connect(model, SIGNAL(EntryAboutToRename(asset::model::Entry*)), this, SLOT(EntryAboutToRename(asset::model::Entry*)));
		connect(model, SIGNAL(EntryRenamed(asset::model::Entry*)), this, SLOT(EntryRenamed(asset::model::Entry*)));

		connect(model, SIGNAL(LocatorAboutToRename(const csResourceLocator &, const csResourceLocator &)), this, SLOT(LocatorAboutToRename(const csResourceLocator &, const csResourceLocator &)));
		connect(model, SIGNAL(LocatorRenamed(const csResourceLocator &, const csResourceLocator &)), this, SLOT(LocatorRenamed(const csResourceLocator &, const csResourceLocator &)));
	}


	void RenameHandler::EntryAboutToRename(asset::model::Entry *entry)
	{
		if (!entry)
		{
			return;
		}
		printf("AboutToRename: %s\n", entry->GetResourceLocator().GetText().c_str());
		m_entries[entry] = entry->GetResourceLocator();
	}

	void RenameHandler::EntryRenamed(asset::model::Entry *entry)
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
		csResourceLocator locator = it->second;
		m_entries.erase(it);
		printf("Renamed: %s => %s\n", locator.GetText().c_str(), entry->GetResourceLocator().GetText().c_str());

		switch (entry->GetType())
		{
		case asset::model::Entry::eT_Asset:
			RenameAsset(entry->AsAsset(), locator);
			break;
		case asset::model::Entry::eT_Folder:
			RenameFolder(entry->AsFolder(), locator);
			break;
		case asset::model::Entry::eT_VFSEntry:
		case asset::model::Entry::eT_Root:
			break;
		}


	}
	void RenameHandler::RenameAsset(asset::model::Asset *asset, csResourceLocator locator)
	{

		std::string fullPath = csVFS::Get()->GetAbsolutePath(locator);
		if (fullPath.empty())
		{
			return;
		}
		std::string newFullPath = csVFS::Get()->GetAbsolutePath(asset->GetResourceLocator(), csVFS::DontCheckExistence);
		if (newFullPath.empty())
		{
			return;
		}


		std::filesystem::path path(fullPath);
		std::filesystem::path newPath(newFullPath);
		if (std::filesystem::exists(path))
		{
			std::filesystem::rename(path, newPath);
		}
	}

	void RenameHandler::RenameFolder(asset::model::Folder *folder, csResourceLocator locator)
	{
		std::string fullPath = csVFS::Get()->GetAbsolutePath(locator);
		if (fullPath.empty())
		{
			return;
		}
		std::string newFullPath = csVFS::Get()->GetAbsolutePath(folder->GetResourceLocator());
		if (newFullPath.empty())
		{
			return;
		}


		std::filesystem::path path(fullPath);
		std::filesystem::path newPath(newFullPath);
		if (std::filesystem::exists(path))
		{
			std::filesystem::rename(path, newPath);
		}
	}



	void RenameHandler::LocatorAboutToRename(const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
	{
		std::set<Entry*> referencing = m_model->GetReferencing(oldLocator);
		if (referencing.empty())
		{
			return;
		}
		m_references[oldLocator] = referencing;
	}

	void RenameHandler::LocatorRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
	{
		auto it = m_references.find(oldLocator);
		if (it == m_references.end())
		{
			return;
		}

		for (Entry *entry : it->second)
		{
			const csResourceLocator &locator = entry->GetResourceLocator();
			std::string fullPath = csVFS::Get()->GetAbsolutePath(locator);
			csfFile file;
			if (file.Parse(fullPath, false))
			{
				Rename(file.GetRoot(), oldLocator, newLocator);
				csfWriter::Write(file, locator);
			}
		}
	}


	void RenameHandler::Rename(csfEntry *entry, const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
	{
		if (entry->HasAttribute("locator"))
		{
			if (entry->GetAttribute("locator") == oldLocator.GetText())
			{
				size_t idx = entry->GetAttributeIndex("locator");
				entry->RemoveAttribute(idx);
				entry->AddAttribute("locator", newLocator.GetText());
			}
		}

		for (size_t i = 0, in = entry->GetNumberOfChildren(); i < in; ++i)
		{
			Rename(entry->GetChild(i), oldLocator, newLocator);
		}
	}
}