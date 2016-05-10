

#include <AssetManager/FolderItemModel.hh>
#include <Valkyrie/Core/VFS.hh>


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

void FolderItemModel::Refresh()
{
  beginResetModel();
  Clear();
  QStringList filters;
  filters.append("*.asset");
  QStringList pathEntries = m_path.entryList(filters, QDir::Files, QDir::Name);
  for (QString &pathEntry : pathEntries)
  {
    Entry *entry = new Entry();
    entry->name = pathEntry;
    if (entry->name.endsWith(".asset"))
    {
      entry->resourceString = m_resourceRel + "/" + pathEntry;
      entry->name = entry->name.left(entry->name.length() - 6);
      entry->icon = vkResourceManager::Get()->Load<EditorIcon>(GetLocator(entry, "EDITOR_ICON"));
    }
    m_entries.append(entry);
  }

  endResetModel();
}

void FolderItemModel::SetPath(const QDir &dir)
{
  m_path = dir;

  QDir rootDir(QString(vkVFS::Get()->GetRootPath().c_str()));
  QString rootPath = rootDir.absolutePath();

  QString dirPath = dir.absolutePath();
  if (dirPath.startsWith(rootPath))
  {
    m_resourceRel = dirPath.right(dirPath.length() - rootPath.length());
  }
  else
  {
    m_resourceRel = "";
  }



  Refresh();
}


void FolderItemModel::Clear()
{
  for (Entry *entry : m_entries)
  {
    VK_RELEASE(entry->icon);
    delete entry;
  }
  m_entries.clear();
}

vkResourceLocator FolderItemModel::GetLocator(const QModelIndex &index, const vkString &name) const
{
  if (index.isValid())
  {
    Entry *entry = static_cast<Entry*>(index.internalPointer());
    return GetLocator(entry, name);
    return vkResourceLocator(
      vkString((const char*)entry->resourceString.toLatin1()),
      name);
  }
  return vkResourceLocator();
}

vkResourceLocator FolderItemModel::GetLocator(Entry *entry, const vkString &name) const
{
  if (entry)
  {
    return vkResourceLocator(
      vkString((const char*)entry->resourceString.toLatin1()),
      name);
  }
  return vkResourceLocator();
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
    else if (role == Qt::DecorationRole)
    {
      if (entry->icon && !entry->icon->GetImage().isNull())
      {
        return entry->icon->GetImage();
      }
      else
      {
        return QImage(":/icons/Resources/NoIcon64.png");
      }
    }
  }
  return QVariant();
}

}


