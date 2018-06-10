
#include <assetmanager/model/viewentry.hh>


namespace asset::model
{

  ViewEntry::ViewEntry (Entry *entry)
    : m_entry(entry)
    , m_parent(0)
  {

  }

  ViewEntry::~ViewEntry()
  {
    m_entry = 0;
  }

}