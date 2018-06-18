
#pragma once

#include <assetmodel/folder.hh>
#include <cobalt/core/csvfs.hh>

namespace asset::model
{
  class Model;
  class VFSEntry : public Folder
  {
  public:
    VFSEntry(Model *model, const csVFS::Entry &entry);
    virtual ~VFSEntry();

    const csVFS::Entry &GetEntry() const { return m_entry; }

    virtual csResourceLocator GetResourceLocator() const;
    virtual csResourceLocator GetNamedResourceLocator(const std::string &name) const;

    virtual const VFSEntry *GetVFSEntry() const;

    int GetPriority() const;

	protected:
		virtual csResourceLocator Construct(const csResourceLocator &parentLocator) const;

  private:
    const csVFS::Entry &m_entry;
  };
}


inline int asset::model::VFSEntry::GetPriority() const
{
  return m_entry.GetPriority();
}
