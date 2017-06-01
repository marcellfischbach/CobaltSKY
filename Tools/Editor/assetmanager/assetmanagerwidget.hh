#pragma once

#include <QDir>
#include <QWidget>
#include <ui_assetmanagerwidget.h>
#include <valkyrie/core/vkresourcelocator.hh>
#include <valkyrie/core/vkstring.hh>
#include <map>

class QAction;
struct AssetManagerNewHandler;
class AssetManagerContentModel;
class AssetManagerFolderModel;
class AssetManagerWidget : public QWidget
{
  Q_OBJECT
public:
  AssetManagerWidget();
  ~AssetManagerWidget();

  void OpenAsset(const vkResourceLocator &locator);

protected slots:

void on_treeView_activated(const QModelIndex &index);
void on_treeView_clicked(const QModelIndex &index);

void on_listView_doubleClicked(const QModelIndex &index);
void on_pbNewAsset_clicked(bool);

private:
  void SelectIndex(const QModelIndex &index);
  Ui::AssetManager m_gui;

  AssetManagerFolderModel *m_folderModel;
  AssetManagerContentModel *m_contentModel;
  QDir m_currentDir;
};

