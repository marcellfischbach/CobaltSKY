#include <editor/importers/importerfileslistmodel.hh>

ImporterFilesListModel::ImporterFilesListModel()
  : QAbstractItemModel ()
{
}

void ImporterFilesListModel::add(const std::string &data)
{
  beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
  m_data.push_back(data);
  endInsertRows();
}


QModelIndex ImporterFilesListModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() || row < 0 || row >= m_data.size())
  {
    return QModelIndex();
  }
  return createIndex(row, column);
}

QModelIndex ImporterFilesListModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int ImporterFilesListModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return 0;
  }
  return (int)m_data.size();
}

int ImporterFilesListModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant ImporterFilesListModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid())
  {
    switch (role)
    {
    case Qt::DisplayRole:
      return QString(m_data[index.row()].c_str());
    }
  }
  return QVariant();
}
