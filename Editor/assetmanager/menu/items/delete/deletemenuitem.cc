
#include <assetmanager/menu/items/delete/deletemenuitem.hh>
#include <assetmanager/menu/items/delete/deletedialog.hh>
#include <assetmanager/menu/menuitemdeclaration.hh>


#include <editormodel/model.hh>
#include <editormodel/nodes/node.hh>

#include <QDialog>
#include <QMenu>
#include <QCursor>

#include <iostream>

namespace  cs::editor::assetmanager
{

DeleteMenuItemHandler::DeleteMenuItemHandler(QAction *action, model::Node *selectedNode, QWidget * parent)
  : MenuItemHandler(action, selectedNode, parent)
{
}

DeleteMenuItemHandler::~DeleteMenuItemHandler()
{
  std::cout << "DeleteMenuItemHandler::~DeleteMenuItemHandler\n";
}

void DeleteMenuItemHandler::OnActionTriggered(bool checked)
{
  std::cout << "DeleteMenuItemHandler::actionTriggered\n";

  model::Model *model = GetModel();
  model::Node *selectedNode = GetSelectedNode();
  std::set<model::Node*> allNodes = model->GetNodes(selectedNode->GetResourceLocator().AsAnonymous());
  std::set<model::Node*> superNodes;
  std::set<model::Node*> overriderNodes;
  int prio = selectedNode->GetPriority();
  for (auto node : allNodes)
  {
    if (node->GetPriority() < prio)
    {
      superNodes.insert(node);
    }
    if (node->GetPriority() > prio)
    {
      overriderNodes.insert(node);
    }
  }

  DeleteDialog dlg(selectedNode, !superNodes.empty(), !overriderNodes.empty(), GetParentWidget());
  dlg.move(QCursor::pos());
  if (dlg.exec() == QDialog::Rejected)
  {
    return;
  }

  model::Transaction tx = model->CreateTransaction();
  try
  {
    tx.Begin();
  }
  catch (std::exception &e)
  {
    std::cerr << "Unable to begin transaction for deleting: " << e.what() << "\n";
    return;
  }

  try
  {
    model->Delete(selectedNode, tx);
    if (dlg.DeleteSuper())
    {
      for (auto super : superNodes)
      {
        model->Delete(super, tx);
      }
    }
    if (dlg.DeleteOverriders())
    {
      for (auto overrider : overriderNodes)
      {
        model->Delete(overrider, tx);
      }
    }
    tx.Commit();
  }
  catch (std::exception &e)
  {
    std::cerr << "Unable to commit transaction for deleting: " << e.what() << "\n";
    try
    {
      tx.Rollback();
    }
    catch (std::exception &ee)
    {
      std::cerr << "Unable to rollback delete transaction: " << ee.what() << "\n";
    }
  }


}

DeleteMenuItemFactory::DeleteMenuItemFactory()
{

}

DeleteMenuItemFactory::~DeleteMenuItemFactory()
{

}

std::vector<MenuItemDeclaration> DeleteMenuItemFactory::GetMenuItemDeclaration() const
{
  std::vector<MenuItemDeclaration> declaration;
  return declaration;
}

QAction *DeleteMenuItemFactory::CreateAction(QMenu *menu, model::Node *selectedNode, QWidget * parent) const
{
  QAction *action = menu->addAction(QObject::tr("Delete"));
  if (!selectedNode || selectedNode->IsRootNode() || selectedNode->IsVFSEntryNode())
  {
    action->setEnabled(false);
  }

  new DeleteMenuItemHandler(action, selectedNode, parent);

  return action;
}

}