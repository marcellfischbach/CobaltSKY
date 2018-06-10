
#include <assetmanager/model/viewroot.hh>
#include <assetmanager/model/viewvfsentry.hh>
#include <assetmodel/entry.hh>
#include <assetmodel/root.hh>
#include <assetmodel/vfsentry.hh>
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
    return p0 < p1;
  }

  void ViewRoot::Sort()
  {
    std::sort(m_entries.begin(), m_entries.end(), lessVFSEntry);
  }
}