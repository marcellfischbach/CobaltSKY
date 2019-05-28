
#pragma once

#include <assetmanager/assetmanagerexport.hh>

#include <QWidget>

namespace cs::editor::model
{
class Model;
}

namespace cs::editor::assetmanager
{

class TreeView;
class ListView;

class ASSET_MANAGER_API AssetManager : public QWidget
{
public:
  enum Style
  {
    eS_Tree,
    eS_List,
    eS_Automatic
  };
public:
  AssetManager(model::Model *model, Style style, QWidget *parent = nullptr);
  virtual ~AssetManager() { }

protected:
  void resizeEvent(QResizeEvent* event);

private:
  ListView* m_list;
  TreeView* m_tree;
  Style m_style;
  QWidget* m_currentWidget;

  int m_minListSize;
};

}