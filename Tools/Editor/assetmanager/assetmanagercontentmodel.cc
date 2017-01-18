

#include <assetmanager/assetmanagercontentmodel.hh>
#include <editor.hh>
#include <QPixmap>

class AssetManagerContentModelEntry
{
public:
  AssetManagerContentModelEntry(const QDir &dir, const QString &fileName)
    : m_dir(dir)
    , m_fileName(fileName)
  {
    m_pixmap = QPixmap(":/icons/resources/UnknownAsset64.png");
  }

  const QString &GetFileName() const
  {
    return m_fileName;
  }

  const QPixmap &GetIcon() const
  {
    return m_pixmap;
  }
    

private:
  QDir m_dir;
  QString m_fileName;
  QPixmap m_pixmap;
};

#define FROM_INDEX(e) reinterpret_cast<AssetManagerContentModelEntry*>(e.internalPointer());

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
      return QVariant(entry->GetFileName());
    case Qt::DecorationRole:
      return QVariant(entry->GetIcon());
    }
  }
  return QVariant();
}

