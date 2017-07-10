
#include <assetmanager/import/assetmanagerimporterlistmodel.hh>
#include <assetmanager/import/assetmanagerimporter.hh>

AssetManagerImporterListModel::AssetManagerImporterListModel()
  : QAbstractItemModel()
{

}

AssetManagerImporterListModel::~AssetManagerImporterListModel()
{

}

void AssetManagerImporterListModel::AddData(AssetManagerImportData* data)
{
  beginResetModel();
  m_datas.push_back(data);
  endResetModel();
}

QModelIndex AssetManagerImporterListModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() || row >= m_datas.size() || column != 0)
  {
    return QModelIndex();
  }

  return createIndex(row, column, m_datas[row]);
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
  return (int)m_datas.size();
}

int AssetManagerImporterListModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant AssetManagerImporterListModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() >= m_datas.size())
  {
    return QVariant();
  }

  if (role == Qt::DisplayRole)
  {
    AssetManagerImportData *data = reinterpret_cast<AssetManagerImportData*>(index.internalPointer());
    return QVariant(data->GetName());
  }
  return QVariant();
}


QModelIndex AssetManagerImporterListModel::GetIndex(AssetManagerImportData *data, int column)
{
  for (int i = 0, in= m_datas.size(); i<in; ++i)
  {
    if (m_datas[i] == data)
    {
      return createIndex(i, column, data);
    }
  }
  return QModelIndex();
}

AssetManagerImportData *AssetManagerImporterListModel::GetData(const QModelIndex &index) const
{
  return reinterpret_cast<AssetManagerImportData*>(index.internalPointer());
}
