
#include <editor/assetmodel/vfsentry.hh>

namespace asset::model
{
  VFSEntry::VFSEntry(Model *model, const csVFS::Entry &entry)
    : Folder(model, Entry::eT_VFSEntry)
    , m_entry(entry)
  {
    SetName(entry.GetName());
  }

  VFSEntry::~VFSEntry()
  {

  }


  csResourceLocator VFSEntry::GetResourceLocator() const
  {
    return csResourceLocator("", "", m_entry.GetName());
  }

  const VFSEntry *VFSEntry::GetVFSEntry() const
  {
    return this;
  }


  bool VFSEntry::IsVFSEntry() const
  {
    return true;
  }

  VFSEntry *VFSEntry::AsVFSEntry()
  {
    return this;
  }

  const VFSEntry *VFSEntry::AsVFSEntry() const
  {
    return this;
  }
}

