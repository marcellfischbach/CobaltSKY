
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


    entry->m_parent = this;
    m_children.push_back(entry);

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
}
