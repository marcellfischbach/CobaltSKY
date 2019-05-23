
#pragma once

#include <editor/editorexport.hh>
#include <editor/assetmodel/folder.hh>
#include <cobalt/core/csvfs.hh>

namespace asset::model
{
  class Model;
  class EDITOR_API VFSEntry : public Folder
  {
  public:
    VFSEntry(Model *model, const cs::VFS::Entry &entry);
    virtual ~VFSEntry();

    const cs::VFS::Entry &GetEntry() const { return m_entry; }

    virtual cs::ResourceLocator GetResourceLocator() const;

    virtual const VFSEntry *GetVFSEntry() const;

    int GetPriority() const;


    virtual bool IsVFSEntry() const;
    virtual VFSEntry *AsVFSEntry();
    virtual const VFSEntry *AsVFSEntry() const;

  private:
    const cs::VFS::Entry &m_entry;
  };
}


inline int asset::model::VFSEntry::GetPriority() const
{
  return m_entry.GetPriority();
}
