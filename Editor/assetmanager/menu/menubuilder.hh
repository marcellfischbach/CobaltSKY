

#pragma once 

class QMenu;
class QWidget;

namespace cs::editor::model
{
class Node;
}

namespace cs::editor::assetmanager
{

class MenuBuilder
{

public:
  static QMenu* Build(model::Node *selectedNode, QWidget * parent);

private:
  QMenu * BuildPriv(model::Node *selectedNode, QWidget * parent);

  MenuBuilder();
};

}