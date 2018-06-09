
#include <assetmodel/entry.hh>
#include <assetmodel/model.hh>

namespace asset::model
{

  Entry::Entry(Model *model, Type type)
    : m_model(model)
    , m_type(type)
  {

  }

  Entry::~Entry()
  {

  }


  void Entry::Add(Entry *entry)
  {


    entry->m_parent = this;
    m_children.push_back(entry);

  }
}
