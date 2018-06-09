
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

  private:
    const csVFS::Entry &m_entry;
  };
}
