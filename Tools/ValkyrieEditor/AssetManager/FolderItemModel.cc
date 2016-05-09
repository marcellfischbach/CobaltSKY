

#include <AssetManager/FolderItemModel.hh>


namespace assetmanager
{


FolderItemModel::FolderItemModel()
  : QAbstractItemModel()
{
  Clear();
}

FolderItemModel::~FolderItemModel()
{

}

void FolderItemModel::SetPath(const QDir &dir)
{
  beginResetModel();
  Clear();

  QStringList filters;
  filters.append("*.asset");
  QStringList pathEntries = dir.entryList(filters, QDir::Files, QDir::Name);
  for (QString &pathEntry : pathEntries)
  {
    Entry *entry = new Entry();
    entry->name = pathEntry;
    if (entry->name.endsWith(".asset"))
    {
      entry->name = entry->name.left(entry->name.length() - 6);
    }
    m_entries.append(entry);
  }

  endResetModel();
}


void FolderItemModel::Clear()
{
  for (Entry *entry : m_entries)
  {
    delete entry;
  }
  m_entries.clear();
}


QModelIndex FolderItemModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!parent.isValid())
  {
    if (row < m_entries.size())
    {
      return createIndex(row, column, m_entries[row]);
    }
  }
  return QModelIndex();
}

QModelIndex FolderItemModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int FolderItemModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
  {
    return m_entries.size();
  }
  return 0;
}

int FolderItemModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant FolderItemModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid() && index.column () == 0)
  {
    Entry *entry = static_cast<Entry*>(index.internalPointer());
    if (role == Qt::DisplayRole)
    {
      return entry->name;
    }
  }
  return QVariant();
}

}


