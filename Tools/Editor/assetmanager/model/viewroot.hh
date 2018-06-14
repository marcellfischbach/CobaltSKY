
#pragma once

#include <assetmanager/model/viewentry.hh>
#include <vector>

namespace asset::model
{

  class Root;
  class ViewVFSEntry;
  class ViewRoot: public ViewEntry
  {
  public:
    ViewRoot(Root* root);
    virtual ~ViewRoot();

    const Root *GetRoot() const;
    Root *GetRoot();

    virtual void Add(ViewEntry *entry);
    virtual void Remove(ViewEntry *entry);
    virtual void Sort();

    const std::vector<ViewEntry*> &GetEntries() const;

  private:

    Root * m_root;

    std::vector<ViewEntry*> m_entries;
  };
}

inline const asset::model::Root *asset::model::ViewRoot::GetRoot() const
{
  return m_root;
}

inline asset::model::Root *asset::model::ViewRoot::GetRoot() 
{
  return m_root;
}

inline const std::vector<asset::model::ViewEntry*> &asset::model::ViewRoot::GetEntries()  const
{
  return m_entries;
}

