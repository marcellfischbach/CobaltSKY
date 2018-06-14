
#include <assetmodel/entry.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>

namespace asset::model
{

  Entry::Entry(Model *model, Type type)
    : m_model(model)
    , m_type(type)
  {

  }

  Entry::~Entry()
  {

  }


  void Entry::Add(Entry *entry)
  {
		if (!entry)
		{
			return;
		}

		emit m_model->EntryAboutToAdd(this, entry);
    entry->m_parent = this;
    m_children.push_back(entry);

		emit m_model->EntryAdded(this, entry);
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
