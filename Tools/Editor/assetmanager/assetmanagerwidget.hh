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
	class Asset;
	class Folder;
	class AssetListModel;
  class FolderTreeModel;
  class TreeModel;
	class ViewDataModel;
	class ViewEntry;
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

  void RefreshContent();

	const std::vector<asset::model::Asset*> GetSelectedAssets() const;

protected slots:

void on_treeView_doubleClicked(const QModelIndex &index);
void on_treeView_customContextMenuRequested(const QPoint &pos);

private:
  Ui::AssetManager m_gui;

  csLogger m_logger;
//  AssetManagerContentItemDelegate *m_itemDelegate;

  asset::model::ViewDataModel *m_dataModel;
  asset::model::TreeModel *m_treeModel;
};

