
#pragma once

#include <vector>
#include <cobalt/core/csresourcelocator.hh>

namespace asset::model
{
  class Model;
	class Folder;
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

    const std::string &GetName() const;

    virtual void Add(Entry *entry);

		virtual bool IsFolder() const;
		virtual Folder *AsFolder();
		virtual const Folder *AsFolder() const;
    bool IsAttached() const;
    virtual csResourceLocator GetResourceLocator() const = 0;

    const std::vector<Entry*> &GetChildren() const;

  protected:
    Entry(Model *model, Type type);

    void SetName(const std::string &name);

  private:
    Model * m_model;
    Type m_type;
    std::string m_name;

    Entry *m_parent;
    std::vector<Entry*> m_children;
  };

}

inline void asset::model::Entry::SetName(const std::string &name)
{
  m_name = name;
}

inline const std::string &asset::model::Entry::GetName() const
{
  return m_name;
}

inline const std::vector<asset::model::Entry*> &asset::model::Entry::GetChildren() const
{
  return m_children;
}
