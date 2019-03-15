

#pragma once


#include <assetmanager/assetmanagerexport.hh>
#include <assetmanager/menu/imenuitemfactory.hh>
#include <assetmanager/menu/menuitemhandler.hh>

namespace cs::editor::assetmanager
{

class NewFolderMenuItemHandler : public MenuItemHandler
{
public:
  NewFolderMenuItemHandler(QAction *action, model::Node *selectedNode, QWidget *parent);
  virtual ~NewFolderMenuItemHandler();

protected:
  virtual void OnActionTriggered(bool checked) override;
};

class ASSET_MANAGER_API NewFolderMenuItemFactory : public IMenuItemFactory
{
public:
  NewFolderMenuItemFactory();
  virtual ~NewFolderMenuItemFactory();

  virtual std::vector<MenuItemDeclaration> GetMenuItemDeclaration() const;

  virtual QAction *CreateAction(QMenu* menu, model::Node* selectedNode, QWidget * parent) const;

};


}