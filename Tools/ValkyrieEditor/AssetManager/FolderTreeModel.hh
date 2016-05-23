#pragma once

#include <qwidget.h>
#include <qdialog.h>
#include <qabstractitemmodel.h>
#include <qdir.h>
#include <qlist.h>
#include <ui_AssetManagerWidget.h>

namespace assetmanager
{

class FolderTreeModel : public QAbstractItemModel
{
public:
  static FolderTreeModel *Get();

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QDir GetDir(const QModelIndex &index) const;
  const QDir &GetRootDir() const;
private:
  FolderTreeModel();

  QDir m_rootDir;

  struct Entry
  {
    QString name;
    QDir dir;
    QList<Entry*> entries;
    Entry *parent;
  };

  void Scan(Entry *parent, const QDir &path);
  int GetIndexOfEntry(Entry *entry) const;

  QList<Entry*> m_entries;
};

}