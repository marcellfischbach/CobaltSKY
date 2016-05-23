#pragma once

#include <qwidget.h>
#include <qdialog.h>
#include <qabstractitemmodel.h>
#include <qdir.h>
#include <qlist.h>
#include <AssetManager/Importer.hh>
#include <ui_AssetManagerWidget.h>

class vkResourceLocator;

namespace assetmanager
{



class FolderItemModel;
class FolderTreeModel;


class AssetManagerWidget : public QWidget
{
  Q_OBJECT;
public:
  AssetManagerWidget(QWidget *parent = 0);
  virtual ~AssetManagerWidget();

private slots:

  void on_pbImport_clicked(bool);
  void on_tvFolders_clicked(const QModelIndex &index);
  void on_lvContent_doubleClicked(const QModelIndex &index);
  void on_lvContent_customContextMenuRequested(const QPoint &pos);

  void OpenSelectedContainer(bool triggered);
  void CloseSelectedContainer(bool triggered);

private:
  Ui::AssetManager m_gui;

  FolderItemModel *m_contentModel;

signals:

  void ResourceActivated(const vkResourceLocator &locator);
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