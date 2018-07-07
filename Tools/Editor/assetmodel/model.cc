
#include <assetmodel/model.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/exceptions.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/modeltransaction.hh>
#include <assetmodel/root.hh>
#include <assetmodel/vfsentry.hh>
#include <assetmodel/sync/deletehandler.hh>
#include <assetmodel/sync/renamehandler.hh>
#include <assetmodel/sync/securefs.hh>

namespace asset::model
{



	Model::Model()
		: QObject()
	{
		m_root = new Root(this);
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

	const Entry *Model::GetEntry(const csResourceLocator &entry) const
	{

		auto it = m_namedEntries.find(entry);
		if (it == m_namedEntries.end())
		{
			return 0;
		}
		return it->second;
	}


	Entry *Model::GetEntry(const csResourceLocator &entry)
	{
		return const_cast<Entry*>(
			static_cast<const Model*>(this)->GetEntry(entry)
			);
	}

	const std::set<Entry*> &Model::GetAllEntriesFor(const csResourceLocator &locator) const
	{
		auto it = m_entries.find(locator.AsAnonymous());
		if (it == m_entries.end())
		{
			throw NoSuchLocatorException(locator.AsAnonymous());
		}
		return it->second;
	}

	std::set<Entry*> Model::GetReferencing(const csResourceLocator &locator) const
	{
		csResourceLocator anonLocator = locator.AsAnonymous();
		std::set<Entry*> entriesReferencingLocator;
		for (auto it = m_references.begin(); it != m_references.end(); ++it)
		{
			if (it->second.find(anonLocator) != it->second.end())
			{
				entriesReferencingLocator.insert(it->first);
			}
		}
		return entriesReferencingLocator;
	}

	int Model::GetLowestPriority(const csResourceLocator &locator) const
	{
		csResourceLocator anonLocator = locator.AsAnonymous();

		auto it = m_entries.find(anonLocator);
		if (it == m_entries.end())
		{
			throw NoSuchLocatorException(locator.GetText());
		}

		int lowestPriority = INT_MAX;
		for (auto entry : it->second)
		{
			const VFSEntry *vfsEntry = entry->GetVFSEntry();
			if (vfsEntry)
			{
				lowestPriority = std::min<int>(lowestPriority, vfsEntry->GetPriority());
			}
		}

		return lowestPriority;
	}

	void Model::Add(asset::model::Entry *parent, asset::model::Entry *child, ModelTransaction &tr)
	{
		if (!parent || !child)
		{
			return;
		}

		parent->AddChild(child);
		InsertIntoEntryCache(child);

    tr.OnCommit([this, parent, child]() {AddCommit(parent, child); });
    tr.OnRollback([this, parent, child]() {AddRollback(parent, child); });
	}

	void Model::AddCommit(Entry *parent, Entry *child)
	{
		if (!parent || !child)
		{
			return;
		}

		emit EntryAdded(parent, child);
	}

	void Model::AddRollback(Entry *parent, Entry *child)
	{
		RemoveFromEntryCache(child);
		parent->RemoveChild(child);
	}

	void Model::Remove(asset::model::Entry *parent, asset::model::Entry *child, ModelTransaction &tr)
	{
		if (!parent || !child || child->GetParent() != parent)
		{
			return;
		}

		csResourceLocator oldLocator = child->GetResourceLocator();
		parent->RemoveChild(child);

    tr.OnCommit([this, parent, child]() { RemoveCommit(parent, child); });
    tr.OnRollback([this, parent, child]() { RemoveRollback(parent, child); });
	}

	void Model::RemoveCommit(asset::model::Entry *parent, asset::model::Entry *child)
	{
		emit EntryRemoved(parent, child);
	}

	void Model::RemoveRollback(asset::model::Entry *parent, asset::model::Entry *child)
	{
		parent->AddChild(child);
	}


	class TreeCollector
	{
	public:
		TreeCollector(Entry *entry)
		{
			if (entry->IsAttached())
			{
				Scan(entry);
			}
		}

		struct Data
		{
			Entry *entry;
			csResourceLocator locator;
			int priority;
			int lowestPriority;
		};

		void Scan(Entry *entry)
		{
			for (Entry *child : entry->GetChildren())
			{
				Scan(child);
			}

			Data d;
			d.entry = entry;
			d.locator = entry->GetResourceLocator();
			d.priority = entry->GetVFSEntryPriority();
			d.lowestPriority = entry->GetModel()->GetLowestPriority(d.locator);

			m_entries.push_back(d);
		}

		std::vector<Data> m_entries;
	};

  

	void Model::Delete(Entry *entry, ModelTransaction &tr)
	{
    for (Entry *child : entry->GetChildren())
    {
      child->Delete(tr);
    }


    csResourceLocator locator = entry->GetResourceLocator();
    tr.OnCommit([this, entry, locator]() { DeleteCommit(entry, locator); });
    entry->RemoveFromParent(tr);

    try
    {
      SecureFS secureFS(tr);
      secureFS.Delete(locator);
    }
    catch (const std::exception &e)
    {
      throw ModelStateException("Unable to delete " + locator.GetText());
    }

	}

  void Model::DeleteCommit(Entry *entry, const csResourceLocator &locator)
  {
    printf("DeleteCommit: %s <- %s\n",
      entry->GetName().c_str(),
      locator.GetText().c_str());
    if (!locator.IsValid())
    {
      return;
    }

    csResourceLocator anonLocator = locator.AsAnonymous();
    emit EntryDeleted(entry, locator);
    if (RemoveFromEntryCache(entry))
    {
      emit ResourceRemoved(anonLocator);
      RemoveReference(anonLocator);
    }

  }


	void Model::Rename(Entry *entry, const std::string &name, ModelTransaction &tr)
	{
		if (!entry->IsAttached())
		{
			return;
		}

		TreeCollector treeCollector(entry);

		csResourceLocator oldLocator = entry->GetResourceLocator();
		csResourceLocator newLocator = entry->GetParent()->GetResourceLocator().WithFileSuffix(entry->FakeName(name));

		try
		{
			SecureFS secureFS(tr);
			secureFS.Move(oldLocator, newLocator);
		}
		catch (const std::exception &e)
		{
			throw ModelStateException(std::string("Unable to move ") + oldLocator.GetText() + std::string(" to ") + newLocator.GetText());
		}

    tr.OnCommit([this, entry, name, treeCollector]() { RenameCommit(entry, name, treeCollector); });
	}

	void Model::RenameCommit(Entry *entry, const std::string &name, TreeCollector treeCollector)
	{
		entry->SetName(name);
		UpdateCollector(treeCollector);
	}

	void Model::UpdateCollector(TreeCollector &treeCollector)
	{
		for (TreeCollector::Data &data : treeCollector.m_entries)
		{
			csResourceLocator oldLocator = data.locator;
			csResourceLocator newLocator = data.entry->GetResourceLocator();

			UpdateCache(data.entry, oldLocator, newLocator);
			emit EntryRenamed(data.entry, oldLocator, newLocator);
		}
		for (TreeCollector::Data &data : treeCollector.m_entries)
		{
			if (data.priority <= data.lowestPriority)
			{
				csResourceLocator oldAnonLocator = data.locator.AsAnonymous();
				csResourceLocator newLocator = data.entry->GetResourceLocator();
				if (!data.entry->IsAttached() || !newLocator.IsValid())
				{
					emit ResourceRemoved(oldAnonLocator);
					RemoveReference(oldAnonLocator);
				}
				else
				{
					csResourceLocator newAnonLocator = newLocator.AsAnonymous();
					emit ResourceRenamed(oldAnonLocator, newAnonLocator);
					RenameReference(oldAnonLocator, newAnonLocator);
				}
			}
		}
	}

	void Model::UpdateCache(Entry *entry, const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
	{
		auto itNE = m_namedEntries.find(oldLocator);
		if (itNE != m_namedEntries.end())
		{
			m_namedEntries.erase(itNE);
		}
		if (newLocator.IsValid())
		{
			m_namedEntries[newLocator] = entry;
		}


		auto itE = m_entries.find(oldLocator.AsAnonymous());
		if (itE != m_entries.end())
		{
			auto itEE = itE->second.find(entry);
			if (itEE != itE->second.end())
			{
				itE->second.erase(itEE);
			}
			if (itE->second.empty())
			{
				m_entries.erase(itE);
			}
		}

		m_entries[newLocator.AsAnonymous()].insert(entry);
			
	}


	void Model::InsertIntoEntryCache(Entry *entry)
	{
		if (!entry)
		{
			return;
		}

		csResourceLocator locator = entry->GetResourceLocator();
		if (!locator.IsValid())
		{
			return;
		}

		m_namedEntries[locator] = entry;
		m_entries[locator.AsAnonymous()].insert(entry);
	}

	bool Model::RemoveFromEntryCache(Entry *entry)
	{
		bool removedLastEntry = false;
		if (!entry)
		{
			return removedLastEntry;
		}

		csResourceLocator locator = EvaluateEntryLocator(entry);
		if (!locator.IsValid())
		{
			return removedLastEntry;
		}

		auto itNE = m_namedEntries.find(locator);
		if (itNE != m_namedEntries.end())
		{
			m_namedEntries.erase(itNE);
		}

		auto itE = m_entries.find(locator.AsAnonymous());
		if (itE != m_entries.end())
		{
			auto itEE = itE->second.find(entry);
			if (itEE != itE->second.end())
			{
				itE->second.erase(itEE);
				if (itE->second.empty())
				{
					m_entries.erase(itE);
					removedLastEntry = true;
				}
			}
		}
		return removedLastEntry;
	}


	void Model::RemoveReference(const csResourceLocator &locator)
	{
		csResourceLocator anonLocator = locator.AsAnonymous();
		std::set<Entry*> &referencingEntries = GetReferencing(anonLocator);

		for (Entry *entry : referencingEntries)
		{
			// reset the reference in all asset-files
		}

		for (auto it = m_references.begin(); it != m_references.end(); ++it)
		{
			auto itR = it->second.find(anonLocator);
			if (itR != it->second.end())
			{
				it->second.erase(itR);
			}
		}
	}

	void Model::RenameReference(const csResourceLocator &olLocator, const csResourceLocator &newLocator)
	{
		csResourceLocator oldAnonLocator = olLocator.AsAnonymous();
		std::set<Entry*> &referencingEntries = GetReferencing(oldAnonLocator);

		for (Entry *entry : referencingEntries)
		{
			// reset the reference in all asset-files
		}

		for (auto it = m_references.begin(); it != m_references.end(); ++it)
		{
			auto itR = it->second.find(oldAnonLocator);
			if (itR != it->second.end())
			{
				it->second.erase(itR);
				it->second.insert(newLocator);
			}
		}
	}

	csResourceLocator Model::EvaluateEntryLocator(const Entry *entry) const
	{
		csResourceLocator locator = entry->GetResourceLocator();
		if (locator.IsValid())
		{
			return locator;
		}

		for (auto namedEntry : m_namedEntries)
		{
			if (namedEntry.second == entry)
			{
				return namedEntry.first;
			}
		}

		return csResourceLocator();
	}


	void Model::ClearReferences(Entry *entry)
	{
		if (!entry)
		{
			return;
		}
		auto it = m_references.find(entry);
		if (it != m_references.end())
		{
			m_references.erase(it);
		}
	}

	void Model::AddReference(Entry *entry, const csResourceLocator &reference)
	{
		if (!entry)
		{
			return;
		}

		m_references[entry].insert(reference.AsAnonymous());

	}

}
