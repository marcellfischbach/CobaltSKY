
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

class ASSET_MANAGER_API TreeView : public QWidget
{

public:
  TreeView(model::Model *model, QWidget *parent = nullptr);


private:
  void InitGUI();

  QLineEdit * m_filter;
  QTreeView * m_treeView;
};

}