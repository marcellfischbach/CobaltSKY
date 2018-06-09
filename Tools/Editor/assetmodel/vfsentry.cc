
#include <assetmodel/vfsentry.hh>

namespace asset::model
{
  VFSEntry::VFSEntry(Model *model, const csVFS::Entry &entry)
    : Folder(model, Entry::eT_VFSEntry)
    , m_entry(entry)
  {

  }

  VFSEntry::~VFSEntry()
  {

  }


  csResourceLocator VFSEntry::GetResourceLocator() const
  {
    return csResourceLocator("", "", m_entry.GetName());
  }

}
