

#pragma once


#include <assetmanager/assetmanagerexport.hh>
#include <assetmanager/menu/imenuitemfactory.hh>
#include <assetmanager/menu/menuitemhandler.hh>

namespace cs::editor::assetmanager
{


class RenameMenuItemHandler : public MenuItemHandler
{
public:
  RenameMenuItemHandler(QAction* action, model::Node *selectedNode, QWidget * parent);
  virtual ~RenameMenuItemHandler();

protected:
  virtual void OnActionTriggered(bool checked) override;
};


class ASSET_MANAGER_API RenameMenuItemFactory : public IMenuItemFactory
{
public:
  RenameMenuItemFactory();
  virtual ~RenameMenuItemFactory();

  virtual std::vector<MenuItemDeclaration> GetMenuItemDeclaration() const;

  virtual QAction *CreateAction(QMenu* menu, model::Node* selectedNode, QWidget * parent) const;

};


}