
#pragma once

#include <list>
#include <cobalt/core/csresourcelocator.hh>

namespace asset::model
{
  class Model;
  class Entry
  {
  public:
    enum Type
    {
      eT_Root,
      eT_VFSEntry,
      eT_Folder,
      eT_Asset
    };

  public:
    virtual ~Entry();
    Entry *GetParent() { return m_parent;  }
    const Entry *GetParent() const { return m_parent; }

    Type GetType() const { return m_type; }

    virtual void Add(Entry *entry);

    bool IsAttached() const;
    virtual csResourceLocator GetResourceLocator() const = 0;

  protected:
    Entry(Model *model, Type type);

  private:
    Model * m_model;
    Type m_type;

    Entry *m_parent;
    std::list<Entry*> m_children;
  };

}