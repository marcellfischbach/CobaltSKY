
#pragma once

#include <assetmanager/assetmanagerexport.hh>
#include <QWidget>

class QLineEdit;
class QTreeView;

namespace cs::editor::model
{
class Model;
}


namespace cs::editor::assetmanager
{

class TreeModel;
class TreeView : public QWidget
{
  Q_OBJECT;
public:
  TreeView(model::Model *model, QWidget *parent = nullptr);

private slots:
  void OnTreeViewCustomContextMenuRequested(const QPoint &pos);

private:
  void InitGUI();

  QLineEdit * m_filter;
  QTreeView * m_treeView;

  TreeModel *m_treeModel;
};

}