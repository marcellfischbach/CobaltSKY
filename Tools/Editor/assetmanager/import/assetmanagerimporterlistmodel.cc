
#include <assetmanager/import/assetmanagerimporterlistmodel.hh>
#include <assetmanager/import/assetmanagerimporter.hh>
#include <assetmanager/import/assetmanagerimporterpage.hh>

AssetManagerImporterListModel::AssetManagerImporterListModel()
  : QAbstractItemModel()
{

}

AssetManagerImporterListModel::~AssetManagerImporterListModel()
{

}

void AssetManagerImporterListModel::AddPage(AssetManagerImporterPage* page)
{
  beginResetModel();
  m_pages.push_back(page);
  endResetModel();
}

void AssetManagerImporterListModel::PageChanged(AssetManagerImporterPage *page)
{
  QModelIndex index = GetIndex(page, 0);
  emit dataChanged(index, index);
}

QModelIndex AssetManagerImporterListModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() || row >= m_pages.size() || column != 0)
  {
    return QModelIndex();
  }

  return createIndex(row, column, m_pages[row]);
}

QModelIndex AssetManagerImporterListModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int AssetManagerImporterListModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return 0;
  }
  return (int)m_pages.size();
}

int AssetManagerImporterListModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant AssetManagerImporterListModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() >= m_pages.size())
  {
    return QVariant();
  }

  AssetManagerImporterPage *page = reinterpret_cast<AssetManagerImporterPage*>(index.internalPointer());
  switch (role)
  {
  case Qt::DisplayRole:
    return QVariant(page->GetName());
  case Qt::FontRole:
    QFont fnt;
    fnt.setItalic(!page->IsImporting());
    return QVariant(fnt);
  }

  return QVariant();
}


QModelIndex AssetManagerImporterListModel::GetIndex(AssetManagerImporterPage *page, int column)
{
  for (int i = 0, in= m_pages.size(); i<in; ++i)
  {
    if (m_pages[i] == page)
    {
      return createIndex(i, column, page);
    }
  }
  return QModelIndex();
}

AssetManagerImporterPage *AssetManagerImporterListModel::GetPage(const QModelIndex &index) const
{
  return reinterpret_cast<AssetManagerImporterPage*>(index.internalPointer());
}
