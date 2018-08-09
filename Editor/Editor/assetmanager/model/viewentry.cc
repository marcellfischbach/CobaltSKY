
#include <assetmanager/model/viewentry.hh>
#include <assetmodel/entry.hh>


namespace asset::model
{

  ViewEntry::ViewEntry (Entry *entry)
    : m_entry(entry)
    , m_parent(0)
  {

  }

  ViewEntry::~ViewEntry()
  {
    m_entry = 0;
  }


	const std::string asset::model::ViewEntry::GetText() const
	{
		return m_entry ? m_entry->GetName() : "<noname>";
	}
}