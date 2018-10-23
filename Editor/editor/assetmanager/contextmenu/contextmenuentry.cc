	

#include <editor/assetmanager/contextmenu/contextmenuentry.hh>
#include <editor/assetmanager/actions/assetmanageractioncallback.hh>
#include <algorithm>


namespace asset::contextmenu
{

	// ///////////////////////////////////////////////////////////////////////
	// ///////////////////////////////////////////////////////////////////////
	// ///////////////////////////////////////////////////////////////////////
	//   Entry

	Entry::Entry(const Category &category)
		: m_category(category)
	{

	}

	// ///////////////////////////////////////////////////////////////////////
	Entry::~Entry()
	{

	}

	// ///////////////////////////////////////////////////////////////////////
	bool Entry::IsFolder() const
	{
		return false;
	}

	// ///////////////////////////////////////////////////////////////////////
	bool Entry::IsAction() const
	{
		return false;
	}


	// ///////////////////////////////////////////////////////////////////////
	Folder *Entry::AsFolder()
	{
		return 0;
	}

	// ///////////////////////////////////////////////////////////////////////
	const Folder *Entry::AsFolder() const
	{
		return 0;
	}

	// ///////////////////////////////////////////////////////////////////////
	Action *Entry::AsAction()
	{
		return 0;
	}

	// ///////////////////////////////////////////////////////////////////////
	const Action *Entry::AsAction() const
	{
		return 0;
	}


	// ///////////////////////////////////////////////////////////////////////
	// ///////////////////////////////////////////////////////////////////////
	// ///////////////////////////////////////////////////////////////////////
	//   Folder


	Folder::Folder(const Category &category, const std::string &name)
		: Entry(category)
		, m_name (name)
	{

	}

	// ///////////////////////////////////////////////////////////////////////
	Folder::~Folder()
	{
		for (Entry *entry : m_entries)
		{
			delete entry;
		}
		m_entries.clear();
	}



	// ///////////////////////////////////////////////////////////////////////
	void Folder::Add(Entry *entry)
	{
		if (std::find(m_entries.begin(), m_entries.end(), entry) != m_entries.end())
		{
			return;
		}
		m_entries.push_back(entry);
	}

	// ///////////////////////////////////////////////////////////////////////
	std::vector<Entry*> Folder::GetEntries()
	{
		return m_entries;
	}

	// ///////////////////////////////////////////////////////////////////////
	const std::vector<Entry*> Folder::GetEntries() const
	{
		return m_entries;
	}

	// ///////////////////////////////////////////////////////////////////////
	const Folder *Folder::Find(const Folder *folder) const
	{
		for (Entry *entry : m_entries)
		{
			if (entry->Matches(folder))
			{
				return entry->AsFolder();
			}
		}
		return 0;
	}

	Folder *Folder::Find(const Folder *folder) 
	{
		return const_cast<Folder*>(
			static_cast<const Folder*>(this)->Find(folder)
			);
	}

	// ///////////////////////////////////////////////////////////////////////
	const std::string &Folder::GetName() const
	{
		return m_name;
	}

	// ///////////////////////////////////////////////////////////////////////
	bool Folder::IsFolder() const
	{
		return true;
	}

	// ///////////////////////////////////////////////////////////////////////
	Folder *Folder::AsFolder()
	{
		return this;
	}
	
	// ///////////////////////////////////////////////////////////////////////
	const Folder *Folder::AsFolder() const
	{
		return this;
	}

	// ///////////////////////////////////////////////////////////////////////
	Folder *Folder::AsNewFolder() const
	{
		return new Folder(GetCategory(), GetName());
	}

	// ///////////////////////////////////////////////////////////////////////
	bool Folder::Matches(const Category &category, const std::string &name) const
	{
		return GetCategory() == category && m_name == name;
	}

	// ///////////////////////////////////////////////////////////////////////
	bool Folder::Matches(const Entry *other) const
	{
		const Folder *otherFolder = other->AsFolder();
		if (!otherFolder)
		{
			return false;
		}
		if (otherFolder == this)
		{
			return true;
		}

		return GetCategory() == otherFolder->GetCategory() && m_name == otherFolder->GetName();
	}

	bool sort_entries(Entry *e0, Entry *e1)
	{
		// check for different types
		if (e0->IsFolder() && e1->IsAction())
		{
			return true;
		}
		else if (e0->IsAction() && e1->IsFolder())
		{
			return false;
		}

		// both types are identical 

		// check whether the category  makes the differences
		if (e0->GetCategory() < e1->GetCategory())
		{
			return true;
		}
		else if (e1->GetCategory() < e0->GetCategory())
		{
			return false;
		}

		if (e0->IsFolder() && e1->IsFolder())
		{
			return e0->AsFolder()->GetName() < e1->AsFolder()->GetName();
		}
		else if (e0->IsAction() && e1->IsAction())
		{
			return e0->AsAction()->GetName() < e1->AsAction()->GetName();
		}
		return false;
	}

	void Folder::Sort()
	{
		std::sort(m_entries.begin(), m_entries.end(), sort_entries);
	}


	// ///////////////////////////////////////////////////////////////////////
	// ///////////////////////////////////////////////////////////////////////
	// ///////////////////////////////////////////////////////////////////////
	//   Action


	Action::Action(const Category &category, const std::string &name)
		: Entry(category)
		, m_name(name)
		, m_action(0)
	{

	}

	// ///////////////////////////////////////////////////////////////////////
	Action::~Action()
	{
		if (m_action)
		{
			delete m_action;
		}
	}



	

	// ///////////////////////////////////////////////////////////////////////
	const std::string &Action::GetName() const
	{
		return m_name;
	}

	// ///////////////////////////////////////////////////////////////////////
	bool Action::IsAction() const
	{
		return true;
	}

	// ///////////////////////////////////////////////////////////////////////
	Action *Action::AsAction()
	{
		return this;
	}

	// ///////////////////////////////////////////////////////////////////////
	const Action *Action::AsAction() const
	{
		return this;
	}

	// ///////////////////////////////////////////////////////////////////////
	Action *Action::AsNewAction() const
	{
		return new Action(GetCategory(), GetName());
	}

	// ///////////////////////////////////////////////////////////////////////
	Action *Action::SetAction(asset::actions::ActionCallback *action)
	{
		m_action = action;
		return this;
	}

	// ///////////////////////////////////////////////////////////////////////
	asset::actions::ActionCallback *Action::GetAction()
	{
		return m_action;
	}

	// ///////////////////////////////////////////////////////////////////////
	const asset::actions::ActionCallback *Action::GetAction() const
	{
		return m_action;
	}

	// ///////////////////////////////////////////////////////////////////////
	bool Action::Matches(const Entry *other) const
	{
		return false;
	}

	// ///////////////////////////////////////////////////////////////////////
	void Action::Callback()
	{
		if (m_action)
		{
			m_action->Callback();
		}
	}

    EntryMergeException::EntryMergeException (const std::string &message)
        : csException(message)
    {

    }
    EntryMergeException::~EntryMergeException ()
    {

    }


	EntryMerger::EntryMerger()
	{

	}

	void EntryMerger::Merge(Entry *parent, const Entry *child)
	{
		if (!parent->IsFolder())
		{
			throw EntryMergeException("Parent must be a folder");
		}

		Folder *parentFolder = parent->AsFolder();

		if (child->IsAction())
		{
			const Action *childAction = child->AsAction();
			parentFolder->Add(childAction->AsNewAction());
		}
		else if (child->IsFolder())
		{
			const Folder *childFolder = child->AsFolder();
			Folder *inTree = parentFolder->Find(childFolder);
			if (!inTree)
			{
				inTree = childFolder->AsNewFolder();
				parentFolder->Add(inTree);
			}

			for (const Entry *childEntry : childFolder->GetEntries())
			{
				Merge(inTree, childEntry);
			}
		}
	}


}
