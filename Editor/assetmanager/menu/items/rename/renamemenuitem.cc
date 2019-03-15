
#include <assetmanager/menu/items/rename/renamemenuitem.hh>
#include <assetmanager/menu/items/rename/renamedialog.hh>
#include <assetmanager/menu/menuitemdeclaration.hh>

#include <editormodel/model.hh>
#include <editormodel/nodes/node.hh>

#include <QMenu>

#include <iostream>

namespace  cs::editor::assetmanager
{



RenameMenuItemHandler::RenameMenuItemHandler(QAction *action, model::Node *selectedNode, QWidget * parent)
  : MenuItemHandler(action, selectedNode, parent)
{
}

RenameMenuItemHandler::~RenameMenuItemHandler()
{
  std::cout << "RenameMenuItemHandler::~RenameMenuItemHandler\n";
}

void RenameMenuItemHandler::OnActionTriggered(bool checked)
{

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
  std::string oldName = selectedNode->GetName();
  RenameDialog dlg(selectedNode, oldName, !superNodes.empty(), !overriderNodes.empty(), GetParentWidget());
  dlg.move(QCursor::pos());
  if (dlg.exec() == QDialog::Rejected)
  {
    return;
  }
  std::string newName = dlg.GetName();
  if (newName.empty() || newName == oldName)
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
    std::cerr << "Unable to begin transaction for renaming: " << e.what() << "\n";
    return;
  }

  try
  {
    model->Rename(selectedNode, newName, tx);
    if (dlg.RenameSuper())
    {
      for (auto super : superNodes)
      {
        model->Rename(super, newName, tx);
      }
    }
    if (dlg.RenameOverriders())
    {
      for (auto overrider : overriderNodes)
      {
        model->Rename(overrider, newName, tx);
      }
    }
    tx.Commit();
  }
  catch (std::exception &e)
  {
    std::cerr << "Unable to commit transaction for renaming: " << e.what() << "\n";
    try
    {
      tx.Rollback();
    }
    catch (std::exception &ee)
    {
      std::cerr << "Unable to rollback renaming transaction: " << e.what() << "\n";
    }
  }


}

RenameMenuItemFactory::RenameMenuItemFactory()
{

}

RenameMenuItemFactory::~RenameMenuItemFactory()
{

}

std::vector<MenuItemDeclaration> RenameMenuItemFactory::GetMenuItemDeclaration() const
{
  std::vector<MenuItemDeclaration> declaration;
  return declaration;
}

QAction *RenameMenuItemFactory::CreateAction(QMenu *menu, model::Node *selectedNode, QWidget * parent) const
{
  QAction *action = menu->addAction(QObject::tr("Rename"));
  if (selectedNode->IsRootNode() || selectedNode->IsVFSEntryNode())
  {
    action->setEnabled(false);
  }
  new RenameMenuItemHandler(action, selectedNode, parent);
  return action;
}

}