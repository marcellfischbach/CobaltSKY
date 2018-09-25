
#include <editor/assetmanager/model/viewroot.hh>
#include <editor/assetmanager/model/viewvfsentry.hh>
#include <editor/assetmodel/entry.hh>
#include <editor/assetmodel/root.hh>
#include <editor/assetmodel/vfsentry.hh>
#include <cobalt/core/csvfs.hh>
#include <algorithm>

namespace asset::model
{
  ViewRoot::ViewRoot(Root *root)
    : ViewEntry(root)
    , m_root(root)
  {

  }

  ViewRoot::~ViewRoot()
  {
    m_root = 0;
    for (ViewEntry *vfsEntry : m_entries)
    {
      delete vfsEntry;
    }
  }



  void ViewRoot::Add(ViewEntry *entry)
  {
    Entry *e = entry->GetEntry();
    if (e->GetType() == Entry::eT_VFSEntry)
    {
      m_entries.push_back(static_cast<ViewVFSEntry*>(entry));
      entry->SetParent(this);
    }
  }



	void ViewRoot::Remove(ViewEntry *entry)
	{
		auto it = std::find(m_entries.begin(), m_entries.end(), entry);
		if (it == m_entries.end())
		{
			return;
		}
		entry->SetParent(0);
		m_entries.erase(it);
	}

  bool lessVFSEntry(ViewEntry *e0, ViewEntry *e1)
  {
    int p0 = 0;
    int p1 = 0;
    if (e0)
    {
      VFSEntry *entry = static_cast<ViewVFSEntry*>(e0)->GetVFSEntry();
      p0 = entry ? entry->GetEntry().GetPriority() : 0;
    }
    if (e1)
    {
      VFSEntry *entry = static_cast<ViewVFSEntry*>(e1)->GetVFSEntry();
      p1 = entry ? entry->GetEntry().GetPriority() : 0;
    }

    if (p0 != p1)
    {
      return p0 < p1;
    }

    std::string n0 = e0->GetText();
    std::string n1 = e1->GetText();
    return n0 < n1;
  }

  void ViewRoot::Sort()
  {
    std::sort(m_entries.begin(), m_entries.end(), lessVFSEntry);
  }
}