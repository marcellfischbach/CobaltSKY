
#pragma once

#include <assetmanager/model/viewentry.hh>
#include <vector>

namespace asset::model
{

  class Folder;
  class ViewAsset;
  class ViewFolder: public ViewEntry
  {
  public:
    ViewFolder(Folder *folder);
    virtual ~ViewFolder();

    const Folder *GetFolder() const;
    Folder *GetFolder();

    virtual void Add(ViewEntry *entry);
    virtual void Sort();

    const std::vector<ViewEntry*> &GetFolders() const;
    const std::vector<ViewEntry*> &GetAssets() const;

  private:
    Folder * m_folder;

    void SortFolders();
    void SortAssets();
    std::vector<ViewEntry*> m_folders;
    std::vector<ViewEntry*> m_assets;


  };
}


inline const asset::model::Folder *asset::model::ViewFolder::GetFolder() const
{
  return m_folder;
}

inline asset::model::Folder *asset::model::ViewFolder::GetFolder() 
{
  return m_folder;
}

inline const std::vector<asset::model::ViewEntry*> &asset::model::ViewFolder::GetFolders() const
{
  return m_folders;
}

inline const std::vector<asset::model::ViewEntry*> &asset::model::ViewFolder::GetAssets() const
{
  return m_assets;
}
