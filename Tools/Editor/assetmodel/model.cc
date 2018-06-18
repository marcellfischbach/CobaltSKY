
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

	const std::set<Entry*> Model::GetEntries(const csResourceLocator &locator) const
	{
		csResourceLocator anonLocator = locator.AsAnonymous();
		auto it = m_entries.find(anonLocator);
		if (it == m_entries.end())
		{
			return std::set<Entry*>();
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

  void Model::RemoveReferences(Entry *entry)
  {
    auto it = m_references.find(entry);
    if (it == m_references.end())
    {
      return;
    }

    m_references.erase(it);
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
	 









	/* ****************************************************************************************/
	/* ****************************************************************************************/
	/*																																												*/
	/*		Modification of the data witin the model																						*/
	/*																																												*/
	/* ****************************************************************************************/
	/* ****************************************************************************************/


	class MoveHelper
	{
	public:
		MoveHelper(Model *model)
			: m_model(model)
		{

		}

		void Collect(Entry *entry)
		{
			const VFSEntry *vfsEntry = entry->GetVFSEntry();

			Node node;
			node.currentLocator = entry->GetResourceLocator();
			node.entry = entry;
			node.priority = vfsEntry ? vfsEntry->GetPriority() : -1;
			node.lowestPriority = m_model->GetLowestPriority(node.currentLocator.AsAnonymous());

			m_nodes.push_back(node);
			for (auto child : entry->GetChildren())
			{
				Collect(child);
			}
		}

		struct Node
		{
			csResourceLocator currentLocator;
			Entry *entry;
			int priority;
			int lowestPriority;
		};

		std::vector<Node> m_nodes;
		Model *m_model;
	};



	void Model::Add(Entry *parent, Entry *child)
	{
		if (!parent || !child)
		{
			return;
		}
		if (child->GetParent() == parent)
		{
			// don't add twice
			return;
		}

		Entry *currentParent = child->GetParent();
		bool move = currentParent && currentParent != parent;
		MoveHelper moveHelper(this);
		if (move)
		{
			moveHelper.Collect(child);
		}


		// now remove the child from the current parent... will do nothing if child is currently not attached to any other entry
		Remove(child->GetParent(), child);

		csResourceLocator currentLocator = child->GetResourceLocator();
		if (move)
		{
			// when moving we must move the files within FS between the possible access... 
			// so we get the future resource locator and move the paths
			csResourceLocator newLocator = parent->GetFutureResourceLocator(child);
			m_sync.Move(currentLocator, newLocator);
		}

		// 
		// attach it back to the graph
		emit EntryAboutToAdd(parent, child);
		parent->AddChild(child);
		emit EntryAdded(parent, child);

		if (move)
		{
			HandleLocatorRenamed(moveHelper);
		}


	}


	void Model::Remove(Entry *parent, Entry *child)
	{
		if (!parent || !child)
		{
			return;
		}

		if (child->GetParent() != parent)
		{
			return;
		}

		emit EntryAboutToRemove(parent, child);
		parent->RemoveChild(child);
		emit EntryRemoved(parent, child);
	}

	void Model::Rename(Entry *entry, const std::string &name)
	{
		MoveHelper moveHelper(this);
		moveHelper.Collect(entry);

		csResourceLocator oldLocator = entry->GetResourceLocator();
		csResourceLocator newLocator = entry->GetNamedResourceLocator(name);
		m_sync.Move(oldLocator, newLocator);
		entry->SetName(name);

		HandleLocatorRenamed(moveHelper);

	}

	void Model::Delete(Entry *entry)
	{

		for (auto child : entry->GetChildren())
		{
			Delete(child);
		}

		csResourceLocator locator = entry->GetResourceLocator();
		entry->RemoveFromParent();
		m_sync.Delete(locator);


		// remove the entry from m_entries and emit signal if this was the last resource with the given name
		csResourceLocator anonLocator = locator.AsAnonymous();
		auto entriesId = m_entries.find(anonLocator);
		if (entriesId != m_entries.end())
		{
			auto entryId = entriesId->second.find(entry);
			if (entryId != entriesId->second.end())
			{
				entriesId->second.erase(entryId);
				if (entriesId->second.empty())
				{
					m_entries.erase(entriesId);
					emit LocatorRemoved(anonLocator);
				}
			}
		}


		// remove the references entry aswell
		auto referenceIt = m_references.find(entry);
		if (referenceIt != m_references.end())
		{
			m_references.erase(referenceIt);
		}


	}

	void Model::HandleLocatorRenamed(MoveHelper &helper)
	{
		for (auto node : helper.m_nodes)
		{
			if (node.priority <= node.lowestPriority)
			{
				emit LocatorRenamed(node.currentLocator, node.entry->GetResourceLocator());
			}
		}
	}


}
