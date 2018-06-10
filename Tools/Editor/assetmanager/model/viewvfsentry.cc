
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
}