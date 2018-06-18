
#include <assetmodel/vfsentry.hh>

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

	csResourceLocator VFSEntry::GetNamedResourceLocator(const std::string &name) const
	{
		return csResourceLocator("", "", name);
	}

	csResourceLocator VFSEntry::Construct(const csResourceLocator &parentLocator) const
	{
		return csResourceLocator("", "", m_entry.GetName());
	}

  const VFSEntry *VFSEntry::GetVFSEntry() const
  {
    return this;
  }

}

