

#include <assetmodel/root.hh>
#include <assetmodel/model.hh>

namespace asset::model
{
  Root::Root(Model *model)
    : Entry(model, Entry::eT_Root)
  {
    SetName("Content");
  }

  Root::~Root()
  {

  }

  void Root::Add(Entry *entry)
  {
    if (!entry)
    {
      return;
    }

    if (entry->GetType() == Entry::eT_VFSEntry)
    {
      Entry::Add(entry);
    }
  }


  csResourceLocator Root::GetResourceLocator() const
  {
    return csResourceLocator();
  }

}
