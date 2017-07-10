#pragma once


#include <QAbstractItemModel>
#include <vector>

struct AssetManagerImportData;

class AssetManagerImporterListModel : public QAbstractItemModel
{
public:
  AssetManagerImporterListModel();
  virtual ~AssetManagerImporterListModel();

  void AddData(AssetManagerImportData* data);

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QModelIndex GetIndex(AssetManagerImportData *graph, int column);
  AssetManagerImportData *GetData(const QModelIndex &index) const;

private:
  std::vector<AssetManagerImportData*> m_datas;
};
