#pragma once

#include <qabstractitemmodel.h>
#include <qdir.h>


namespace assetmanager
{


class FolderItemModel : public QAbstractItemModel
{
public:
  FolderItemModel();
  virtual ~FolderItemModel();


  void SetPath(const QDir &path);

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
  void Clear();
  struct Entry
  {
    QString name;
  };

  QList<Entry*> m_entries;
};

}