

#include <editor/assetmodel/root.hh>
#include <editor/assetmodel/model.hh>

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

  void Root::Add(Entry *entry, ModelTransaction &tr)
  {
    if (!entry)
    {
      return;
    }

    if (entry->GetType() == Entry::eT_VFSEntry)
    {
      Entry::Add(entry, tr);
    }
  }

  void Root::Delete(ModelTransaction &tr)
  {
    // root may never be deleted.
  }

  bool Root::IsAttached() const
  {
    return true;
  }

  cs::ResourceLocator Root::GetResourceLocator() const
  {
    return cs::ResourceLocator();
  }

	bool Root::IsRoot() const
	{
		return true;
	}

	Root *Root::AsRoot()
	{
		return this;
	}

	const Root *Root::AsRoot() const
	{
		return this;
	}
}
