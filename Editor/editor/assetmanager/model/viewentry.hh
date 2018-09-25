

#pragma once

#include <editor/editorexport.hh>
#include <string>

namespace asset::model
{

  class Entry;
  class ViewEntry
  {
  public:
    virtual ~ViewEntry();

    const Entry *GetEntry() const;
    Entry *GetEntry();

    void SetParent(ViewEntry* parent);
    const ViewEntry *GetParent() const;
    ViewEntry *GetParent();

		virtual const std::string GetText() const;

    virtual void Add(ViewEntry *entry) = 0;
    virtual void Remove(ViewEntry *entry) = 0;
    virtual void Sort() = 0;

  protected:
    ViewEntry(Entry* entry);


  private:
    ViewEntry * m_parent;

    Entry * m_entry;
  };

}


inline const asset::model::Entry *asset::model::ViewEntry::GetEntry() const
{
  return m_entry;
}

inline asset::model::Entry *asset::model::ViewEntry::GetEntry()
{
  return m_entry;
}

inline void asset::model::ViewEntry::SetParent(ViewEntry *parent)
{
  m_parent = parent;
}

inline asset::model::ViewEntry *asset::model::ViewEntry::GetParent()
{
  return m_parent;
}

inline const asset::model::ViewEntry *asset::model::ViewEntry::GetParent() const
{
  return m_parent;
}
