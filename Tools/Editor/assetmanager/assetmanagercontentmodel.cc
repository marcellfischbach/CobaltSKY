

#include <assetmanager/assetmanagercontentmodel.hh>
#include <editor.hh>
#include <QPixmap>
#include <QMimeData>

class AssetManagerContentModelEntry
{
public:
  AssetManagerContentModelEntry(const QDir &dir, const QString &fileName)
    : m_dir(dir)
    , m_entryName(fileName)
  {
    m_pixmap = QPixmap(":/icons/resources/UnknownAsset64.png");
    QString fn = m_dir.absoluteFilePath(m_entryName);
    m_fileName = (const char*)fn.toLatin1();
  }

  vkString GetFileName() const
  {
    return  m_fileName;
    
  }

  const QPixmap &GetIcon() const
  {
    return m_pixmap;
  }

  const QString &GetEntryName() const
  {
    return m_entryName;
  }
    

private:
  QDir m_dir;
  QString m_entryName;
  QPixmap m_pixmap;
  vkString m_fileName;
};

#define FROM_INDEX(e) reinterpret_cast<AssetManagerContentModelEntry*>(e.internalPointer());
#define CONST_FROM_INDEX(e) reinterpret_cast<const AssetManagerContentModelEntry*>(e.internalPointer());

AssetManagerContentModel::AssetManagerContentModel()
  : QAbstractItemModel()
{
  SetDir(Editor::Get()->GetRootPath());
}

AssetManagerContentModel::~AssetManagerContentModel()
{
  CleanupEntries();
}

void AssetManagerContentModel::SetDir(const QDir &dir)
{
  if (dir == m_dir)
  {
    return;
  }
  beginResetModel();
  CleanupEntries();

  m_dir = dir;
  QStringList nameFilters;
  nameFilters.append("*.xasset");
  QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);
  for (const QString &fileName : fileNames)
  {
    m_entries.push_back(new AssetManagerContentModelEntry(m_dir, fileName));
  }

  endResetModel();
}

void AssetManagerContentModel::CleanupEntries()
{
  for (AssetManagerContentModelEntry *entry : m_entries)
  {
    delete entry;
  }
  m_entries.clear();
}

QModelIndex AssetManagerContentModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() || column != 0)
  {
    return QModelIndex();
  }

  return createIndex(row, 0, m_entries[row]);
}

QModelIndex AssetManagerContentModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int AssetManagerContentModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return 0;
  }
  return m_entries.size();
}

int AssetManagerContentModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant AssetManagerContentModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid())
  {
    AssetManagerContentModelEntry *entry = FROM_INDEX(index);
    switch (role)
    {
    case Qt::DisplayRole:
      return QVariant(entry->GetEntryName());
    case Qt::DecorationRole:
      return QVariant(entry->GetIcon());
    }
  }
  return QVariant();
}

QStringList AssetManagerContentModel::mimeTypes() const
{
  printf("mimeTypes\n");
  QStringList result;
  result << "valkyrie_editor/asset_resource";
  return result;
}

QMimeData *AssetManagerContentModel::mimeData(const QModelIndexList &indexes) const
{
  printf("mimeData: %d\n", indexes.size());
  if (indexes.size() != 1)
  {
    return 0;
  }
  const QModelIndex &index = indexes[0];
  const AssetManagerContentModelEntry *entry = CONST_FROM_INDEX(index);

  QMimeData *mimeData = new QMimeData();
  mimeData->setData(QString("valkyrie_editor/asset_resource"), QString(entry->GetFileName().c_str()).toLatin1());
  return mimeData;
}

Qt::ItemFlags AssetManagerContentModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

  if (index.isValid())
    return Qt::ItemIsDragEnabled | defaultFlags;
  else
    return defaultFlags;
}

vkString AssetManagerContentModel::GetEntry(const QModelIndex &index) const
{
  const AssetManagerContentModelEntry *entry = CONST_FROM_INDEX(index);
  if (!entry)
  {
    return "";
  }
  return entry->GetFileName();
}

