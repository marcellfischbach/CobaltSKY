#pragma once


#include <editor/editorexport.hh>
#include <QAbstractItemModel>
#include <QDir>
#include <cobalt/core/csresourcelocator.hh>

class AssetManagerFolderModelEntry;

class AssetManagerFolderModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  AssetManagerFolderModel();
  virtual ~AssetManagerFolderModel();

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QDir GetDir(const QModelIndex &index) const;
  cs::ResourceLocator GetResourceLocator(const QModelIndex &index) const;

private:

  AssetManagerFolderModelEntry * m_root;
};