
#include <assetmanager/model/viewdatamodel.hh>
#include <assetmanager/model/viewasset.hh>
#include <assetmanager/model/viewfolder.hh>
#include <assetmanager/model/viewroot.hh>
#include <assetmanager/model/viewvfsentry.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>
#include <assetmodel/root.hh>
#include <assetmodel/vfsentry.hh>



namespace asset::model
{

  ViewDataModel::ViewDataModel()
    : QObject()
    , m_model(0)
    , m_root(0)
  {

  }

  ViewDataModel::~ViewDataModel()
  {

  }


  void ViewDataModel::SetModel(Model *model)
  {
    m_model = model;
    Rebuild();
  }

  void ViewDataModel::Cleanup()
  {
    if (m_root)
    {
      delete m_root;
      m_root = 0;
    }

    m_data.clear();

  }

  void ViewDataModel::Rebuild()
  {
    Cleanup();
    if (!m_model)
    {
      return;
    }
    m_root = static_cast<ViewRoot*>(Build(m_model->GetRoot ()));
  }

  ViewEntry *ViewDataModel::Build(Entry *entry)
  {
    ViewEntry *viewEntry = Create(entry);
    if (!viewEntry)
    {
      return 0;
    }

    for (Entry *childEntry: entry->GetChildren())
    {
      ViewEntry *viewChildEntry = Build(childEntry);
      if (viewChildEntry)
      {
        viewEntry->Add(viewChildEntry);
      }
    }

    viewEntry->Sort();

    return viewEntry;
  }


  ViewEntry *ViewDataModel::Create(Entry *entry)
  {
    if (!entry)
    {
      return 0;
    }

    switch (entry->GetType())
    {
    case Entry::eT_Asset:
      return new ViewAsset(static_cast<Asset*>(entry));
    case Entry::eT_Folder:
      return new ViewFolder(static_cast<Folder*>(entry));
    case Entry::eT_VFSEntry:
      return new ViewVFSEntry(static_cast<VFSEntry*>(entry));
    case Entry::eT_Root:
      return new ViewRoot(static_cast<Root*>(entry));
    }
    return 0;
  }



}