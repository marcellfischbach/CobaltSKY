
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>

namespace asset::model
{
  Folder::Folder(Model *model, const std::string &folderName)
    : Entry(model, Entry::eT_Folder)
    , m_folderName(folderName)
  {

  }

  Folder::Folder(Model *model, Entry::Type type)
    : Entry(model, type)
  {

  }

  Folder::~Folder()
  {

  }

  csResourceLocator Folder::GetResourceLocator() const
  {
    const Entry *parent = GetParent();
    if (parent)
    {
      return parent->GetResourceLocator().WithFileSuffix(m_folderName + "/");
    }
    return csResourceLocator();
  }

  void Folder::Add(Entry *entry)
  {
    if (!entry)
    {
      return;
    }

    if (entry->GetType() == Entry::eT_Folder || entry->GetType() == Entry::eT_Asset)
    {
      Entry::Add(entry);
    }
  }
}
