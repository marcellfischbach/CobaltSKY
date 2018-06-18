
#include <assetmodel/entry.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>

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


  void Entry::Add(Entry *entry)
  {
		m_model->Add(this, entry);
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


	void Entry::Remove(Entry *entry)
	{
		m_model->Remove(this, entry);
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

	void Entry::RemoveFromParent()
	{
		if (!m_parent)
		{
			return;
		}
		m_parent->Remove(this);
	}


	void Entry::Delete()
	{
		m_model->Delete(this);
	}



	void Entry::Rename(const std::string &name)
	{
		m_model->Rename(this, name);
	}

	bool Entry::IsAttached() const
	{
		if (!m_parent)
		{
			return false;
		}
		return m_parent->IsAttached();
	}

	csResourceLocator Entry::GetFutureResourceLocator(Entry *child) const
	{
		if (!IsAttached() || !child)
		{
			return csResourceLocator();
		}

		return child->Construct(GetResourceLocator());
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
