
#pragma once

#include <assetmanager/assetmanagerexport.hh>
#include <QWidget>

class QLineEdit;
class QTreeView;
class QListView;
class QSplitter;

namespace cs::editor::model
{
class Model;
}


namespace cs::editor::assetmanager
{

class ListTreeModel;
class ListListModel;
class ListView : public QWidget
{
  Q_OBJECT;
public:
  ListView(model::Model* model, QWidget* parent = nullptr);

private:
  void InitGUI();

  QTreeView* m_treeView;
  QListView* m_listView;
  QSplitter* m_splitter;

  ListTreeModel* m_treeModel;
  ListListModel* m_listModel;

private slots:
  void on_treeView_SelectionChanged(const QModelIndex& idx, const QModelIndex &);
};

}