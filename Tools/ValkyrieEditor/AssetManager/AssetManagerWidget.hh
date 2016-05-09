#pragma once

#include <qwidget.h>
#include <qdialog.h>
#include <qabstractitemmodel.h>
#include <qdir.h>
#include <ui_AssetManagerWidget.h>

namespace assetmanager
{

class FolderItemModel;
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
private:
  FolderTreeModel();


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



class AssetManagerWidget : public QWidget
{
  Q_OBJECT;
public:
  AssetManagerWidget(QWidget *parent = 0);
  virtual ~AssetManagerWidget();

private slots:

  void on_tvFolders_clicked(const QModelIndex &index);

private:
  Ui::AssetManager m_gui;

  FolderItemModel *m_contentModel;
};


class AssetManagerDialog : public QDialog
{
public:
  AssetManagerDialog(QWidget *parent = 0);
  ~AssetManagerDialog();

private:
  AssetManagerWidget *m_widget;

};


}