
#include <assetmanager/model/viewvfsentry.hh>
#include <assetmodel/vfsentry.hh>

namespace asset::model
{
  ViewVFSEntry::ViewVFSEntry(VFSEntry *entry)
    : ViewFolder(entry)
    , m_vfsEntry(entry)
  {

  }

  ViewVFSEntry::~ViewVFSEntry()
  {

  }

	const std::string ViewVFSEntry::GetText() const
	{
		if (m_vfsEntry)
		{
			return m_vfsEntry->GetName() + " (" + std::to_string(m_vfsEntry->GetEntry().GetPriority()) + ")";
		}
		return ViewEntry::GetText();
	}
}