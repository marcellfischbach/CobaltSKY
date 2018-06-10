
#include <assetmanager/model/foldertreemodel.hh>
#include <assetmanager/model/viewdatamodel.hh>
#include <assetmanager/model/viewentry.hh>
#include <assetmanager/model/viewroot.hh>
#include <assetmanager/model/viewfolder.hh>
#include <assetmanager/model/viewvfsentry.hh>
#include <assetmodel/entry.hh>

namespace asset::model
{

  FolderTreeModel::FolderTreeModel()
    : QAbstractItemModel()
    , m_dataModel(0)
  {

  }

  FolderTreeModel::~FolderTreeModel()
  {

  }

  void FolderTreeModel::SetViewDataModel(ViewDataModel *dataModel)
  {
    beginResetModel();
    m_dataModel = dataModel;
    endResetModel();
  }

  QModelIndex FolderTreeModel::index(int row, int column, const QModelIndex &parent) const
  {
    if (!parent.isValid())
    {
      if (row == 0 && m_dataModel)
      {
        return createIndex(row, column, m_dataModel->GetRoot());
      }
      return QModelIndex();
    }

    ViewEntry *parentEntry = reinterpret_cast<ViewEntry*>(parent.internalPointer());
    if (!parentEntry)
    {
      return QModelIndex();
    }

    switch (parentEntry->GetEntry()->GetType())
    {
    case Entry::eT_Asset:
      return QModelIndex();
    case Entry::eT_Root:
      return createIndex(row, column, GetChild(static_cast<ViewRoot*>(parentEntry), row));
    case Entry::eT_Folder:
    case Entry::eT_VFSEntry:
      return createIndex(row, column, GetChild(static_cast<ViewFolder*>(parentEntry), row));
    }

    return QModelIndex();
  }

  QModelIndex FolderTreeModel::parent(const QModelIndex &child) const
  {
    if (!child.isValid())
    {
      return QModelIndex();
    }

    ViewEntry *childEntry = reinterpret_cast<ViewEntry*>(child.internalPointer());
    if (!childEntry)
    {
      return QModelIndex();
    }
  
    ViewEntry *parentEntry = childEntry->GetParent();
    ViewEntry *grandParentEntry = parentEntry ? parentEntry->GetParent() : 0;
    
    int indexOfParent = IndexOf(grandParentEntry, parentEntry);
    return createIndex(indexOfParent, child.column(), parentEntry);
  }

  int FolderTreeModel::rowCount(const QModelIndex &parent) const
  {
    if (!parent.isValid())
    {
      return 1;
    }

    ViewEntry *parentEntry = reinterpret_cast<ViewEntry*>(parent.internalPointer());
    if (!parentEntry)
    {
      return 0;
    }

    switch (parentEntry->GetEntry()->GetType())
    {
    case Entry::eT_Asset:
      return 0;
    case Entry::eT_Root:
      return static_cast<ViewRoot*>(parentEntry)->GetEntries().size();
    case Entry::eT_Folder:
    case Entry::eT_VFSEntry:
      return static_cast<ViewFolder*>(parentEntry)->GetFolders().size();
    }

    return 0;
  }

  int FolderTreeModel::columnCount(const QModelIndex &parent) const
  {
    return 1;
  }

  QVariant FolderTreeModel::data(const QModelIndex &index, int role) const
  {
    if (!index.isValid())
    {
      return QVariant();
    }
    if (role != Qt::DisplayRole)
    {
      return QVariant();
    }

    ViewEntry *entry = reinterpret_cast<ViewEntry*>(index.internalPointer());
    return QString(entry->GetEntry()->GetName().c_str());
  }




  ViewEntry * FolderTreeModel::GetChild(ViewEntry *entry, size_t idx) const
  {
    if (!entry || !entry->GetEntry())
    {
      return 0;
    }
    switch (entry->GetEntry()->GetType())
    {
    case Entry::eT_Root:
      return static_cast<ViewRoot*>(entry)->GetEntries()[idx];

    case Entry::eT_VFSEntry:
    case Entry::eT_Folder:
      return static_cast<ViewFolder*>(entry)->GetFolders()[idx];

    case Entry::eT_Asset:
      return 0;
    }
    return 0;
  }


  int FolderTreeModel::IndexOf(ViewEntry *parent, ViewEntry *child) const
  {
    if (!parent || !parent->GetEntry())
    {
      return 0;
    }

    switch (parent->GetEntry()->GetType())
    {
    case Entry::eT_Root:
    {
      const std::vector<ViewEntry*> &rootChildren = static_cast<ViewRoot*>(parent)->GetEntries();
      return std::distance(rootChildren.begin(), std::find(rootChildren.begin(), rootChildren.end(), child));
    }
    case Entry::eT_VFSEntry:
    case Entry::eT_Folder:
    {
      const std::vector<ViewEntry*> &folderChildren = static_cast<ViewFolder*>(parent)->GetFolders();
      return std::distance(folderChildren.begin(), std::find(folderChildren.begin(), folderChildren.end(), child));
    }

    case Entry::eT_Asset:
    {
      return 0;
    }
    }
    return 0;
  }

}