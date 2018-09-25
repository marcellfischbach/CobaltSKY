#pragma once


#include <editor/editorexport.hh>
#include <QAbstractItemModel>
#include <vector>

class AssetManagerImporterPage;

class AssetManagerImporterListModel : public QAbstractItemModel
{ 
public:
  AssetManagerImporterListModel();
  virtual ~AssetManagerImporterListModel();

  void AddPage(AssetManagerImporterPage* page);
  void PageChanged(AssetManagerImporterPage* page);

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QModelIndex GetIndex(AssetManagerImporterPage *page, int column);
  AssetManagerImporterPage *GetPage(const QModelIndex &index) const;

private:
  std::vector<AssetManagerImporterPage*> m_pages;
};
