
#include <editor/assetmodel/entry.hh>
#include <editor/assetmodel/exceptions.hh>
#include <editor/assetmodel/folder.hh>
#include <editor/assetmodel/model.hh>
#include <editor/assetmodel/modeltransaction.hh>
#include <editor/assetmodel/vfsentry.hh>

namespace asset::model
{

  Entry::Entry(Model *model, Type type)
    : m_model(model)
    , m_type(type)
    , m_parent(0)
  {

  }

  Entry::~Entry()
  {

  }


	void Entry::SetDisplayName(const std::string &name)
	{
		SetName(name);
	}

	const std::string &Entry::GetDisplayName() const
	{
		return GetName();
	}


	bool Entry::IsDescendentOf(const Entry *entry) const
	{
		const Entry *test = this;
		while (test)
		{
			if (test == entry)
			{
				return true;
			}
			test = test->GetParent();
		}
		return false;
	}

	const std::string Entry::FakeName(const std::string &name) const
	{
		return name;
	}


  void Entry::Add(Entry *entry, ModelTransaction &tr)
  {
		m_model->Add(this, entry, tr);
	}

	bool Entry::AddChild(Entry *child)
	{
		if (!child)
		{
			return false;
		}
		if (child->m_parent)
		{
			return false;
		}
		auto it = std::find(m_children.begin(), m_children.end(), child);
		if (it != m_children.end())
		{
			return false;
		}

		child->m_parent = this;
		m_children.push_back(child);
		return true;
	}


	void Entry::Remove(Entry *entry, ModelTransaction &tr)
	{
		m_model->Remove(this, entry, tr);
	}

	bool Entry::RemoveChild(Entry *child)
	{
		if (!child)
		{
			return false;
		}

    child->m_parent = nullptr;
		auto it = std::find(m_children.begin(), m_children.end(), child);
		if (it == m_children.end())
		{
			return false;
		}
		m_children.erase(it);
    child->m_parent = nullptr;
		return true;
	}

	void Entry::MoveTo(Entry *newParent, ModelTransaction &tr)
	{
		m_model->Move(GetParent(), newParent, this, tr);
	}



	void Entry::RemoveFromParent(ModelTransaction &tr)
	{
		if (!m_parent)
		{
			return;
		}
		m_parent->Remove(this, tr);
	}


	void Entry::Delete(ModelTransaction &tr)
	{
		m_model->Delete(this, tr);
	}



	void Entry::Rename(const std::string &name, ModelTransaction &tr)
	{
		m_model->Rename(this, name, tr);
	}

	bool Entry::IsAttached() const
	{
		if (!m_parent)
		{
			return false;
		}
		return m_parent->IsAttached();
	}

	csResourceLocator Entry::CreateResourceLocator(const std::string &fileName) const
	{
		if (!IsAttached())
		{
			return csResourceLocator();
		}
		return GetResourceLocator().WithFileSuffix(fileName);
	}

	bool Entry::ContainsChild(const std::string &name) const
	{
		for (const Entry *entry : m_children)
		{
			if (name == entry->GetName())
			{
				return true;
			}
		}
		return false;
	}

	Entry *Entry::GetChildByName(const std::string &name)
	{
		return const_cast<Entry*>(
			static_cast<const Entry*>(this)->GetChildByName(name)
			);
	}

	const Entry *Entry::GetChildByName(const std::string &name) const
	{
		for (const Entry *entry : m_children)
		{
			if (name == entry->GetName())
			{
				return entry;
			}
		}
		return 0;
	}

	Folder *Entry::FindFolder()
	{
		return const_cast<Folder*>(
			static_cast<const Entry*>(this)->FindFolder()
			);
	}

	const Folder *Entry::FindFolder() const
	{
		const Entry *entry = this;
		while (entry)
		{
			const Folder *folder = entry->AsFolder();
			if (folder)
			{
				return folder;
			}
			entry = entry->GetParent();
		}

		return 0;
	}

  const VFSEntry *Entry::GetVFSEntry() const
  {
    if (m_parent)
    {
      return m_parent->GetVFSEntry();
    }
    return 0;
  }

	int Entry::GetVFSEntryPriority() const
	{
		const VFSEntry *vfsEntry = GetVFSEntry();
		if (!vfsEntry)
		{
			throw NotAttachedException(this);
		}
		return vfsEntry->GetPriority();

	}

	bool Entry::IsFolder() const
	{
		return false;
	}

	Folder *Entry::AsFolder()
	{
		return 0;
	}

	const Folder *Entry::AsFolder() const
	{
		return 0;
	}

	bool Entry::IsAsset() const
	{
		return false;
	}

	Asset *Entry::AsAsset()
	{
		return 0;
	}

	const Asset *Entry::AsAsset() const
	{
		return 0;
	}


  bool Entry::IsVFSEntry() const
  {
    return false;
  }

  VFSEntry *Entry::AsVFSEntry()
  {
    return 0;
  }

  const VFSEntry *Entry::AsVFSEntry() const
  {
    return 0;
  }

	bool Entry::IsRoot() const
	{
		return false;
	}

	Root *Entry::AsRoot()
	{
		return 0;
	}

	const Root *Entry::AsRoot() const
	{
		return 0;
	}
}
