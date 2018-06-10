#pragma once

#include <QDir>
#include <QWidget>
#include <ui_assetmanagerwidget.h>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/cslogger.hh>
#include <map>
#include <string>

namespace asset::model
{
  class ViewDataModel;
  class FolderTreeModel;
}
class Project;
class QAction;
struct AssetManagerNewHandler;
class AssetManagerContentItemDelegate;
class AssetManagerContentModel;
class AssetManagerContentModelEntry;
class AssetManagerFolderModel;
class AssetManagerWidget : public QWidget
{
  Q_OBJECT
public:
  AssetManagerWidget();
  ~AssetManagerWidget();

  void SetProject(Project *project);

  void OpenAsset(const csResourceLocator &locator);

  void RefreshContent();

  const csResourceLocator &GetContentResource() const;
  csResourceLocator GetContentResource(const std::string &resourceFile, const std::string &resourceName = "") const;
  QString GetNewAssetName(const QString &baseName) const;
  QString GetFilePath(const QString &fileName) const;
  const QList<const AssetManagerContentModelEntry*> GetSelectedAssets() const;

protected slots:

void on_treeView_activated(const QModelIndex &index);
void on_treeView_clicked(const QModelIndex &index);

void on_listView_doubleClicked(const QModelIndex &index);
void on_listView_customContextMenuRequested(const QPoint &pos);
void on_pbNewAsset_clicked(bool);
void on_pbImport_clicked(bool);

private slots:
void on_importDialogAccepted ();

private:
  void FillStdMenu(QMenu *menu);
  void FillNewMenu(QMenu *menu);
  void SelectIndex(const QModelIndex &index);
  Ui::AssetManager m_gui;

  AssetManagerFolderModel *m_folderModel;
  AssetManagerContentModel *m_contentModel;
  QDir m_currentDir;
  csLogger m_logger;
  AssetManagerContentItemDelegate *m_itemDelegate;

  asset::model::ViewDataModel *m_dataModel;
  asset::model::FolderTreeModel *m_folderTreeModel;
};

