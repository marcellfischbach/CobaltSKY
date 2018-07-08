
#include <assetmanager/model/treemodel.hh>
#include <assetmanager/model/viewdatamodel.hh>
#include <assetmanager/model/viewentry.hh>
#include <assetmanager/model/viewroot.hh>
#include <assetmanager/model/viewfolder.hh>
#include <assetmanager/model/viewvfsentry.hh>
#include <assetmodel/entry.hh>
#include <QApplication>
#include <QStyle>
#include <QIcon>

namespace asset::model
{

  TreeModel::TreeModel()
    : QAbstractItemModel()
    , m_dataModel(0)
  {

  }

  TreeModel::~TreeModel()
  {

  }

  void TreeModel::SetViewDataModel(ViewDataModel *dataModel)
  {
    if (m_dataModel)
    {
      DisconnectDataModel();
    }
    beginResetModel();
    m_dataModel = dataModel;
    endResetModel();
    if (m_dataModel)
    {
      ConnectDataModel();
    }
  }

  void TreeModel::ConnectDataModel()
  {
    connect(m_dataModel, SIGNAL(EntryAdded(asset::model::ViewEntry*, asset::model::ViewEntry*)), this, SLOT(EntryAdded(asset::model::ViewEntry*, asset::model::ViewEntry*)));
    connect(m_dataModel, SIGNAL(EntryAboutToRemove(asset::model::ViewEntry*, asset::model::ViewEntry*)), this, SLOT(EntryAboutToRemove(asset::model::ViewEntry*, asset::model::ViewEntry*)));
    connect(m_dataModel, SIGNAL(EntryRemoved(asset::model::ViewEntry*, asset::model::ViewEntry*)), this, SLOT(EntryRemoved(asset::model::ViewEntry*, asset::model::ViewEntry*)));
  }

  void TreeModel::DisconnectDataModel()
  {
    disconnect(m_dataModel, SIGNAL(EntryAdded(asset::model::ViewEntry*, asset::model::ViewEntry*)), this, SLOT(EntryAdded(asset::model::ViewEntry*, asset::model::ViewEntry*)));
    disconnect(m_dataModel, SIGNAL(EntryAboutToRemove(asset::model::ViewEntry*, asset::model::ViewEntry*)), this, SLOT(EntryAboutToRemove(asset::model::ViewEntry*, asset::model::ViewEntry*)));
    disconnect(m_dataModel, SIGNAL(EntryRemoved(asset::model::ViewEntry*, asset::model::ViewEntry*)), this, SLOT(EntryRemoved(asset::model::ViewEntry*, asset::model::ViewEntry*)));
  }

  QModelIndex TreeModel::GetRootModelIndex() const
  {
    return createIndex(0, 0, m_dataModel->GetRoot());
  }

  QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
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
      return createIndex(row, column, GetChildOfRoot(static_cast<ViewRoot*>(parentEntry), row));
    case Entry::eT_Folder:
    case Entry::eT_VFSEntry:
      return createIndex(row, column, GetChildOfFolder(static_cast<ViewFolder*>(parentEntry), row));
    }

    return QModelIndex();
  }

  QModelIndex TreeModel::parent(const QModelIndex &child) const
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
    if (!parentEntry)
    {
      return QModelIndex();
    }
    ViewEntry *grandParentEntry = parentEntry ? parentEntry->GetParent() : 0;
    int indexOfParent = IndexOf(grandParentEntry, parentEntry);
    return createIndex(indexOfParent, child.column(), parentEntry);
  }

  int TreeModel::rowCount(const QModelIndex &parent) const
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
      return static_cast<ViewFolder*>(parentEntry)->GetFolders().size() + static_cast<ViewFolder*>(parentEntry)->GetAssets().size();
    }

    return 0;
  }

  int TreeModel::columnCount(const QModelIndex &parent) const
  {
    return 1;
  }

  QVariant TreeModel::data(const QModelIndex &index, int role) const
  {
    if (!index.isValid())
    {
      return QVariant();
    }
    ViewEntry *entry = reinterpret_cast<ViewEntry*>(index.internalPointer());
    if (!entry)
    {
      return QVariant();
    }
    switch (role)
    {
    case Qt::DisplayRole:
      return QString(entry->GetText().c_str());

    case Qt::DecorationRole:
      if (entry->GetEntry()->IsAsset())
      {
        return QApplication::style()->standardIcon(QStyle::SP_FileIcon);
      }
      if (entry->GetEntry()->IsFolder())
      {
        return QApplication::style()->standardIcon(QStyle::SP_DirIcon);
      }
      break;
    }
    return QVariant();
  }


  ViewEntry *TreeModel::GetEntry(const QModelIndex &index)
  {
    return const_cast<ViewEntry*>(
      static_cast<const TreeModel*>(this)->GetEntry(index)
      );
  }

  const ViewEntry *TreeModel::GetEntry(const QModelIndex &index) const
  {
    if (!index.isValid())
    {
      return 0;
    }
    return reinterpret_cast<const ViewEntry*>(index.internalPointer());
  }


  ViewEntry * TreeModel::GetChildOfFolder(ViewFolder *folder, size_t idx) const
  {
    if (!folder || !folder->GetEntry())
    {
      return 0;
    }

    const std::vector<asset::model::ViewEntry*> &folders = folder->GetFolders();
    if (idx < folders.size())
    {
      return folders[idx];
    }

    idx -= folders.size();
    const std::vector<asset::model::ViewEntry*> &assets = folder->GetAssets();
    if (idx < assets.size())
    {
      return assets[idx];
    }
    return 0;
  }

  ViewEntry * TreeModel::GetChildOfRoot(ViewRoot *root, size_t idx) const
  {
    if (!root || !root->GetEntry())
    {
      return 0;
    }

    const std::vector<asset::model::ViewEntry*> &entries = root->GetEntries();
    if (idx < entries.size())
    {
      return entries[idx];
    }

    return 0;
  }


  int TreeModel::IndexOf(ViewEntry *parent, ViewEntry *child) const
  {
    if (!parent || !parent->GetEntry())
    {
      return 0;
    }

    switch (parent->GetEntry()->GetType())
    {
    case Entry::eT_Root:
      return IndexOfRootParent(static_cast<ViewRoot*>(parent), child);
    {
      const std::vector<ViewEntry*> &rootChildren = static_cast<ViewRoot*>(parent)->GetEntries();
      return std::distance(rootChildren.begin(), std::find(rootChildren.begin(), rootChildren.end(), child));
    }
    case Entry::eT_VFSEntry:
    case Entry::eT_Folder:
      return IndexOfFolderParent(static_cast<ViewFolder*>(parent), child);
    {
      const std::vector<ViewEntry*> &folderChildren = static_cast<ViewFolder*>(parent)->GetFolders();
      int idx = std::distance(folderChildren.begin(), std::find(folderChildren.begin(), folderChildren.end(), child));
    }

    case Entry::eT_Asset:
    {
      return 0;
    }
    }
    return 0;
  }

  int TreeModel::IndexOfRootParent(ViewRoot *root, ViewEntry *child) const
  {
    const std::vector<ViewEntry*> &children = root->GetEntries();
    auto it = std::find(children.begin(), children.end(), child);
    if (it == children.end())
    {
      return -1;
    }

    return std::distance(children.begin(), it);
  }

  int TreeModel::IndexOfFolderParent(ViewFolder *folder, ViewEntry *child) const
  {
    const std::vector<ViewEntry*> &folderChildren = folder->GetFolders();
    auto it = std::find(folderChildren.begin(), folderChildren.end(), child);
    if (it != folderChildren.end())
    {
      return std::distance(folderChildren.begin(), it);
    }

    const std::vector<ViewEntry*> &assetChildren = folder->GetAssets();
    it = std::find(assetChildren.begin(), assetChildren.end(), child);
    if (it != assetChildren.end())
    {
      return std::distance(assetChildren.begin(), it);
    }


    return -1;
  }


  void TreeModel::EntryAdded(ViewEntry *parent, ViewEntry *child)
  {
    int idx = IndexOf(parent, child);
    if (idx == -1)
    {
      return;
    }

    ViewEntry *grandParent = parent ? parent->GetParent() : 0;
    int indexOfParent = IndexOf(grandParent, parent);
    beginInsertRows(createIndex(indexOfParent, 0, parent), idx, idx);
    endInsertRows();
  }

  void TreeModel::EntryAboutToRemove(ViewEntry *parent, ViewEntry *child)
  {
    int idx = IndexOf(parent, child);
    if (idx == -1)
    {
      return;
    }

    ViewEntry *grandParent = parent ? parent->GetParent() : 0;
    int indexOfParent = IndexOf(grandParent, parent);
    beginRemoveRows(createIndex(indexOfParent, 0, parent), idx, idx);
  }

  void TreeModel::EntryRemoved(ViewEntry *parent, ViewEntry *child)
  {
    endRemoveRows();
  }
}