
#pragma once

#include <vector>

class QAction;
class QMenu;
class QWidget;


namespace cs::editor::model
{

class Node;
}

namespace cs::editor::assetmanager
{

class MenuItemDeclaration;

struct IMenuItemFactory
{
  virtual ~IMenuItemFactory() { }

  virtual std::vector<MenuItemDeclaration> GetMenuItemDeclaration() const = 0;

  virtual QAction *CreateAction(QMenu* menu, model::Node *selectedNode, QWidget *parent) const = 0;
};


}