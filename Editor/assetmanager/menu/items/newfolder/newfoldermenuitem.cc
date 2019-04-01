
#include <assetmanager/menu/items/newfolder/newfoldermenuitem.hh>
#include <assetmanager/menu/items/newfolder/newfolderdialog.hh>
#include <assetmanager/menu/menuitemdeclaration.hh>

#include <editormodel/nodes/foldernode.hh>
#include <editormodel/nodes/node.hh>
#include <editormodel/model.hh>
#include <editormodel/fs/secfs.hh>

#include <cobalt/core/csvfs.hh>

#include <QMenu>

#include <filesystem>
#include <iostream>

namespace cs::editor::assetmanager
{

NewFolderMenuItemHandler::NewFolderMenuItemHandler(QAction *action, model::Node *selectedNode, QWidget *parent)
  : MenuItemHandler(action, selectedNode, parent)
{
}

NewFolderMenuItemHandler::~NewFolderMenuItemHandler()
{
  std::cout << "NewFolderMenuItemHandler::~NewFolderMenuItemHandler\n";
}

void NewFolderMenuItemHandler::OnActionTriggered(bool)
{
  std::cout << "NewFolderMenuItemHandler::OnActionTriggered\n";
  NewFolderDialog dlg(GetParentWidget());
  dlg.move(QCursor::pos());
  if (dlg.exec() == QDialog::Rejected)
  {
    return;
  }

  std::string folderName = dlg.GetFolderName();
  if (folderName.empty())
  {
    return;
  }

  model::Model *model = GetModel();
  model::Node* selectedNode = GetSelectedNode();
  model::FolderNode *parentFolderNode = selectedNode->GetFolderNode();
  if (!parentFolderNode)
  {
    return;
  }

  std::string absolutePath = csVFS::Get()->GetAbsolutePath(parentFolderNode->GetResourceLocator(), csVFS::CheckExistence);
  if (absolutePath.empty())
  {
    return;
  }

  std::filesystem::path path(absolutePath);
  path /= folderName;

  model::Transaction tx = GetModel()->CreateTransaction();
  try 
  {
    tx.Begin();
  }
  catch (std::exception &e)
  {
    std::cerr << "Unable to begin transaction: " << e.what() << "\n";
  }
  try
  {
    model::SecureFS().CreateFolder(path, tx);

    model::FolderNode *folderNode = model->CreateFolderNode();
    folderNode->SetName(dlg.GetFolderName());
    model->Add(folderNode, parentFolderNode, tx);


    tx.Commit();
  }
  catch (std::exception &e)
  {
    std::cerr << "Unable to commit transaction: " << e.what() << "\n";
    try
    {
      tx.Rollback();
    }
    catch (std::exception &ee)
    {
      std::cerr << "Unable to rollback transaction: " << ee.what() << "\n";
    }
  }

}

NewFolderMenuItemFactory::NewFolderMenuItemFactory()
{

}

NewFolderMenuItemFactory::~NewFolderMenuItemFactory()
{

}

std::vector<MenuItemDeclaration> NewFolderMenuItemFactory::GetMenuItemDeclaration() const
{
  std::vector<MenuItemDeclaration> declaration;
  declaration.push_back(MenuItemDeclaration::New());
  return declaration;
}

QAction *NewFolderMenuItemFactory::CreateAction(QMenu *menu, model::Node *selectedNode, QWidget * parent) const
{
  QAction *action = menu->addAction(QIcon(":/icons16/folder_closed"), QObject::tr("Folder"));
  if (selectedNode->IsRootNode())
  {
    action->setEnabled(false);
  }
  new NewFolderMenuItemHandler(action, selectedNode, parent);
  return action;
}

}