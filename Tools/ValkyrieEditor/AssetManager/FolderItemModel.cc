

#include <AssetManager/FolderItemModel.hh>
#include <Valkyrie/Core/VFS.hh>
#include <AssetManager/Utils.hh>
#include <QMimeData>


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
  filters.append("*.xasset");
  QStringList pathEntries = m_path.entryList(filters, QDir::Dirs | QDir::Files, QDir::Name);
  for (QString &pathEntry : pathEntries)
  {
    Entry *entry = new Entry();
    entry->name = pathEntry;
    if (entry->name.endsWith(".xasset"))
    {
      entry->resourceString = m_resourceRel + "/" + pathEntry;
      entry->displayName = entry->name.left(entry->name.length() - 6);
      entry->icon = vkResourceManager::Get()->Load<EditorIcon>(GetLocator(entry, "preview"));
      QDir dir(m_path);
      entry->container = dir.cd(pathEntry);
      entry->containerOpen = false;
      entry->parent = 0;
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

vkResourceLocator FolderItemModel::CreateLocator (const QString &assetName) const
{
  QString path = m_resourceRel + "/" + assetName + ".asset";
  return vkResourceLocator(vkString((const char*)path.toLatin1()));
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
      return entry->displayName;
    }
    else if (role == Qt::DecorationRole)
    {
      if (entry->icon && !entry->icon->GetImage().isNull())
      {
        return entry->icon->GetImage();
      }
      else if (entry->container)
      {
        return entry->containerOpen
          ? QImage(":/icons/Resources/ContainerOpen64.png")
          : QImage(":/icons/Resources/ContainerClose64.png");

      }
      else
      {
        return QImage(":/icons/Resources/NoIcon64.png");
      }
    }
  }
  return QVariant();
}

bool FolderItemModel::IsContainer(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return false;
  }

  Entry *entry = static_cast<Entry*>(index.internalPointer());
  return entry->container;
}

bool FolderItemModel::IsContainerOpen(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return false;
  }

  Entry *entry = static_cast<Entry*>(index.internalPointer());
  return entry->container && entry->containerOpen;
}

void FolderItemModel::CloseContainer(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return;
  }
  Entry *entry = static_cast<Entry*>(index.internalPointer());

  while (true)
  {
    bool removed = false;
    for (unsigned i = 0, in = m_entries.size(); i < in; ++i)
    {
      Entry *e = m_entries[i];
      if (e->parent == entry)
      {
        beginRemoveRows(QModelIndex(), i, i);
        m_entries.removeAt(i);
        delete e;
        endRemoveRows();
        removed = true;
        break;
      }
    }
    if (!removed)
    {
      break;
    }
  }
  entry->containerOpen = false;
}

void FolderItemModel::OpenContainer(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return;
  }
  Entry *entry = static_cast<Entry*>(index.internalPointer());

  int idx = m_entries.indexOf(entry) + 1;

  QStringList filters;
  filters.append("*.asset");
  QDir path(m_path);
  path.cd(entry->name);
  QStringList pathEntries = path.entryList(filters, QDir::Files, QDir::Name);
  for (QString &pathEntry : pathEntries)
  {
    Entry *newEntry = new Entry();
    newEntry->name = pathEntry;
    if (newEntry->name.endsWith(".asset"))
    {
      newEntry->resourceString = entry->resourceString + "/" + pathEntry;
      newEntry->displayName = newEntry->name.left(newEntry->name.length() - 6);
      newEntry->icon = vkResourceManager::Get()->Load<EditorIcon>(GetLocator(newEntry, "EDITOR_ICON"));
      newEntry->container = false;
      newEntry->containerOpen = false;
      newEntry->parent = entry;
    }

    beginInsertRows(QModelIndex(), idx, idx);
    m_entries.insert(idx, newEntry);
    endInsertRows();
  }
  entry->containerOpen = true;

}

Qt::ItemFlags FolderItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    flags |= Qt::ItemIsDragEnabled;

    return flags;
}

QMimeData *FolderItemModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.size() != 1)
    {
        return 0;
    }

    const QModelIndex &index = indexes.at(0);
    Entry *entry = static_cast<Entry*>(index.internalPointer());

    QString type = assetmanager::GetTypeOfResource(vkResourceLocator(vkString((const char*)entry->resourceString.toLatin1())));


    QMimeData *mimeData = new QMimeData();
    mimeData->setData("VALKYRIE/RESOURCE/TYPE", type.toLatin1());
    mimeData->setData("VALKYRIE/RESOURCE/FILE", entry->resourceString.toLatin1());
    return mimeData;
}

}


