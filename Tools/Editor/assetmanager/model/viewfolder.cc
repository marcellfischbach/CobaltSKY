
#include <assetmanager/model/viewfolder.hh>
#include <assetmanager/model/viewasset.hh>
#include <assetmodel/folder.hh>
#include <algorithm>



namespace asset::model
{

  ViewFolder::ViewFolder(Folder *folder)
    : ViewEntry(folder)
    , m_folder(folder)
  {


  }

  ViewFolder::~ViewFolder()
  {
    m_folder = 0;
    for (ViewEntry *folder : m_folders)
    {
      delete folder;
    }

    for (ViewEntry *asset : m_assets)
    {
      delete asset;
    }
  }

  void ViewFolder::Add(ViewEntry *entry)
  {
    Entry *e = entry->GetEntry();
    if (!e)
    {
      return;
    }

    if (e->GetType() == Entry::eT_Asset)
    {
      m_assets.push_back(static_cast<ViewAsset*>(entry));
      entry->SetParent(this);
    }
    else if (e->GetType() == Entry::eT_Folder)
    {
      m_folders.push_back(static_cast<ViewFolder*>(entry));
      entry->SetParent(this);
    }

  }

	void ViewFolder::Remove(ViewEntry *entry)
	{
		auto it = std::find(m_assets.begin(), m_assets.end(), entry);
		if (it != m_assets.end())
		{
			entry->SetParent(0);
			m_assets.erase(it);
		}
		it = std::find(m_folders.begin(), m_folders.end(), entry);
		if (it != m_folders.end())
		{
			entry->SetParent(0);
			m_folders.erase(it);
		}
	}

  void ViewFolder::Sort()
  {
    SortAssets();
    SortFolders();
  }

  static bool entryLess(ViewEntry *e0, ViewEntry *e1)
  {
    if (e0 && e1)
    {
      return e0->GetEntry()->GetName() < e1->GetEntry()->GetName();
    }
    return false;
  }

  void ViewFolder::SortAssets()
  {
    std::sort(m_assets.begin(), m_assets.end(), entryLess);
  }

  void ViewFolder::SortFolders()
  {
    std::sort(m_folders.begin(), m_folders.end(), entryLess);
  }

}