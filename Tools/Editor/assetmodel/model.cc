
#include <assetmodel/model.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/root.hh>
#include <assetmodel/vfsentry.hh>
#include <assetmodel/sync/deletehandler.hh>
#include <assetmodel/sync/renamehandler.hh>

namespace asset::model
{
	Model::Model()
		: QObject()
	{
		m_root = new Root(this);
		m_deleteHandler = new sync::DeleteHandler(this);
		m_renameHandler = new sync::RenameHandler(this);

		connect(this, SIGNAL(EntryAdded(asset::model::Entry*, asset::model::Entry*)), SLOT(onEntryAdded(asset::model::Entry*, asset::model::Entry*)));
		connect(this, SIGNAL(EntryRemoved(asset::model::Entry*, asset::model::Entry*)), SLOT(onEntryRemoved(asset::model::Entry*, asset::model::Entry*)));
		connect(this, SIGNAL(EntryRenamed(asset::model::Entry*)), SLOT(onEntryRenamed(asset::model::Entry*)));
		connect(this, SIGNAL(LocatorRenamed(const csResourceLocator &, const csResourceLocator &)), SLOT(onLocatorRenamed(const csResourceLocator &, const csResourceLocator &)));
	}

	Model::~Model()
	{

	}


	Asset *Model::CreateAsset(const std::string &fileName)
	{
		return new Asset(this, fileName);
	}

	Folder *Model::CreateFolder(const std::string &folderName)
	{
		return new Folder(this, folderName);
	}

	VFSEntry *Model::CreateVFSEntry(const csVFS::Entry &entry)
	{
		return new VFSEntry(this, entry);
	}

	static std::set<Entry*> EmptyEntrySet;

	const std::set<Entry*> &Model::GetEntries(const csResourceLocator &locator) const
	{
		csResourceLocator anonLocator = locator.AsAnonymous();
		auto it = m_entries.find(anonLocator);
		if (it == m_entries.end())
		{
			return EmptyEntrySet;
		}
		return it->second;
	}

	const std::set<Entry*> Model::GetReferencing(const csResourceLocator &locator) const
	{
		csResourceLocator anonLocator = locator.AsAnonymous();

		std::set<Entry*> referencing;
		for (auto entry : m_references)
		{
			auto it = entry.second.find(anonLocator);
			if (it != entry.second.end())
			{
				referencing.insert(entry.first);
			}
		}
		return referencing;
	}

	void Model::AddReference(Entry *entry, const csResourceLocator &locator)
	{
		if (!entry || !locator.IsValid())
		{
			return;
		}
		csResourceLocator anonLocator = locator.AsAnonymous();
		m_references[entry].insert(anonLocator);
	}

	void Model::onEntryAdded(Entry *parent, Entry *child)
	{
		csResourceLocator locator = child->GetResourceLocator();
		csResourceLocator anonLocator = locator.AsAnonymous();
		m_entries[anonLocator].insert(child);
	}

	void Model::onEntryRemoved(Entry *parent, Entry *child)
	{
		for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
		{
			auto mit = it->second.find(child);
			if (mit != it->second.end())
			{
				it->second.erase(mit);
				if (it->second.empty())
				{
					m_entries.erase(it);
				}
				return;
			}
		}
	}

	csResourceLocator Model::GetCurrentResourceLocator(Entry *entry) const
	{
		for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
		{
			auto mit = it->second.find(entry);
			if (mit != it->second.end())
			{
				return it->first;
			}
		}
		return csResourceLocator();
	}

	int Model::GetLowestPriority(const csResourceLocator &locator) const
	{
		int lowest = INT_MAX;
		auto it = m_entries.find(locator);
		if (it != m_entries.end())
		{

			for (auto entry : it->second)
			{
				int entryPriority = entry->GetVFSEntry()->GetPriority();
				lowest = entryPriority < lowest ? entryPriority : lowest;
			}
		}
		return lowest;
	}

	void Model::onEntryRenamed(Entry *entry)
	{
		csResourceLocator currentLocator = GetCurrentResourceLocator(entry);
		csResourceLocator newLocator = entry->GetResourceLocator().AsAnonymous();
		int entryPriority = entry->GetVFSEntry()->GetPriority();
		int lowestPriority = GetLowestPriority(currentLocator);

		if (entryPriority <= lowestPriority)
		{
			// referencing resource locators must be renamed aswell.
			emit LocatorAboutToRename(currentLocator, newLocator);
		}

		auto oldEntriesIt = m_entries.find(currentLocator);
		if (oldEntriesIt != m_entries.end())
		{
			auto oldIt = oldEntriesIt->second.find(entry);
			if (oldIt != oldEntriesIt->second.end())
			{
				oldEntriesIt->second.erase(oldIt);
				if (oldEntriesIt->second.empty())
				{
					m_entries.erase(oldEntriesIt);
				}
			}
		}

		m_entries[newLocator].insert(entry);

		if (entryPriority <= lowestPriority)
		{
			emit LocatorRenamed(currentLocator, newLocator);
		}

	}


	void Model::onLocatorRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
	{
		for (auto entry : m_references)
		{
			auto it = entry.second.find(oldLocator);
			if (it != entry.second.end())
			{
				entry.second.erase(it);
				entry.second.insert(newLocator);
			}
		}
	}
}
