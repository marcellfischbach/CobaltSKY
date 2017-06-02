#pragma once

#include <QDir>
#include <QWidget>
#include <ui_assetmanagerwidget.h>
#include <valkyrie/core/vkresourcelocator.hh>
#include <valkyrie/core/vkstring.hh>
#include <map>
#include <valkyrie/core/vklogger.hh>

class QAction;
struct AssetManagerNewHandler;
class AssetManagerContentModel;
class AssetManagerContentModelEntry;
class AssetManagerFolderModel;
class AssetManagerWidget : public QWidget
{
  Q_OBJECT
public:
  AssetManagerWidget();
  ~AssetManagerWidget();

  void OpenAsset(const vkResourceLocator &locator);

  const QList<const AssetManagerContentModelEntry*> GetSelectedAssets() const;

protected slots:

void on_treeView_activated(const QModelIndex &index);
void on_treeView_clicked(const QModelIndex &index);

void on_listView_doubleClicked(const QModelIndex &index);
void on_listView_customContextMenuRequested(const QPoint &pos);
void on_pbNewAsset_clicked(bool);

private:
  void FillStdMenu(QMenu *menu);
  void FillNewMenu(QMenu *menu);
  void SelectIndex(const QModelIndex &index);
  Ui::AssetManager m_gui;

  AssetManagerFolderModel *m_folderModel;
  AssetManagerContentModel *m_contentModel;
  QDir m_currentDir;
  vkLogger m_logger;
};

