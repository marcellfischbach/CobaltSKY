#pragma once
#include <QWidget>
#include <ui_assetmanagerwidget.h>
#include <valkyrie/core/vkstring.hh>

class AssetManagerContentModel;
class AssetManagerFolderModel;
class AssetManagerWidget : public QWidget
{
  Q_OBJECT
public:
  AssetManagerWidget();
  ~AssetManagerWidget();

  void OpenAsset(const vkString &filename);

protected slots:

void on_treeView_activated(const QModelIndex &index);
void on_treeView_clicked(const QModelIndex &index);

void on_listView_doubleClicked(const QModelIndex &index);


private:
  void SelectIndex(const QModelIndex &index);
  Ui::AssetManager m_gui;

  AssetManagerFolderModel *m_folderModel;
  AssetManagerContentModel *m_contentModel;
};
