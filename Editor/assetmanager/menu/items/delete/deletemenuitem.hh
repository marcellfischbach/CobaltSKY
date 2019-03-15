

#pragma once


#include <assetmanager/assetmanagerexport.hh>
#include <assetmanager/menu/imenuitemfactory.hh>
#include <assetmanager/menu/menuitemhandler.hh>
#include <QObject>

namespace cs::editor::assetmanager
{

class DeleteMenuItemHandler : public MenuItemHandler
{
public:
  DeleteMenuItemHandler(QAction* action, model::Node *selectedNode, QWidget * parent);
  virtual ~DeleteMenuItemHandler();

protected:
  virtual void OnActionTriggered(bool checked) override;
};


class ASSET_MANAGER_API DeleteMenuItemFactory : public IMenuItemFactory
{
public:
  DeleteMenuItemFactory();
  virtual ~DeleteMenuItemFactory();

  virtual std::vector<MenuItemDeclaration> GetMenuItemDeclaration() const;

  virtual QAction *CreateAction(QMenu* menu, model::Node* selectedNode, QWidget * parent) const;

};


}