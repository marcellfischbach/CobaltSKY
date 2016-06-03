
#include <AssetManager/FolderTreeModel.hh>


namespace assetmanager
{

FolderTreeModel::FolderTreeModel()
  : QAbstractItemModel()
{
#if 0
  m_rootDir = QDir("G:/ide/devs/valkyrieengine/data");
#else
    m_rootDir = QDir("D:/programming/valkyrieengine/data");
#endif
  Scan(0, m_rootDir);
}

FolderTreeModel *FolderTreeModel::Get()
{
  static FolderTreeModel model;
  return &model;
}



QModelIndex FolderTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    Entry *entry = static_cast<Entry*>(parent.internalPointer());
    if (row < entry->entries.size())
    {
      return createIndex(row, column, entry->entries[row]);
    }
  }
  else
  {
    if (row < m_entries.size())
    {
      return createIndex(row, column, m_entries[row]);
    }
  }
  return QModelIndex();
}

QModelIndex FolderTreeModel::parent(const QModelIndex &child) const
{
  if (child.isValid())
  {
    Entry *entry = static_cast<Entry*>(child.internalPointer());
    if (entry->parent)
    {
      return createIndex(GetIndexOfEntry(entry->parent), child.column(), entry->parent);
    }
  }
  return QModelIndex();
}

int FolderTreeModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    Entry *entry = static_cast<Entry*>(parent.internalPointer());
    return entry->entries.size();
  }
  return m_entries.size();
}

int FolderTreeModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant FolderTreeModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid())
  {
    Entry *entry = static_cast<Entry*>(index.internalPointer());
    if (role == Qt::DisplayRole && index.column() == 0)
    {
      return entry->name;
    }
  }
  return QVariant();
}


int FolderTreeModel::GetIndexOfEntry(Entry *entry) const
{
  if (entry->parent)
  {
    return entry->parent->entries.indexOf(entry);
  }
  return m_entries.indexOf(entry);
}


void FolderTreeModel::Scan(Entry *parent, const QDir &path)
{
  Entry *entry = new Entry();
  entry->name = path.dirName();
  entry->dir = path;
  entry->parent = parent;
  entry->entries.clear();

  if (parent)
  {
    parent->entries.append(entry);
  }
  else
  {
    m_entries.append(entry);
  }

  QStringList entries = path.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  for (QString pathEntry : entries)
  {
    if (pathEntry.endsWith(".asset"))
    {
      continue;
    }
    QDir nextPath(path);
    if (nextPath.cd(pathEntry))
    {
      Scan(entry, nextPath);
    }
  }
}

QDir FolderTreeModel::GetDir(const QModelIndex &index) const
{
  if (index.isValid())
  {
    Entry *entry = static_cast<Entry*>(index.internalPointer());
    return entry->dir;
  }
  return QDir();
}

const QDir &FolderTreeModel::GetRootDir() const
{
  return m_rootDir;
}

}
