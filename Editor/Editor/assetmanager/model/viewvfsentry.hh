
#pragma once

#include <assetmanager/model/viewfolder.hh>

namespace asset::model
{

  class VFSEntry;

  class ViewVFSEntry: public ViewFolder
  {
  public:
    ViewVFSEntry(VFSEntry* vfsEntry);
    virtual ~ViewVFSEntry();

    const VFSEntry *GetVFSEntry() const;
    VFSEntry *GetVFSEntry();

		virtual const std::string GetText() const;

  private:
    VFSEntry * m_vfsEntry;
  };
}


inline const asset::model::VFSEntry* asset::model::ViewVFSEntry::GetVFSEntry() const
{
  return m_vfsEntry;
}

inline asset::model::VFSEntry* asset::model::ViewVFSEntry::GetVFSEntry() 
{
  return m_vfsEntry;
}
