
#include <assetmodel/entry.hh>
#include <assetmodel/exceptions.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>
#include <assetmodel/modeltransaction.hh>
#include <assetmodel/vfsentry.hh>

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

		child->m_parent = 0;
		auto it = std::find(m_children.begin(), m_children.end(), child);
		if (it == m_children.end())
		{
			return false;
		}
		m_children.erase(it);
		child->m_parent = 0;
		return true;
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
}
