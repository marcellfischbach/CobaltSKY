

#include <assetmanager/menu/menubuilder.hh>
#include <assetmanager/menu/menuitemdeclaration.hh>
#include <assetmanager/menu/menuitemfactories.hh>
#include <assetmanager/menu/imenuitemfactory.hh>

#include <QMenu>

namespace cs::editor::assetmanager
{


MenuBuilder::MenuBuilder()
{

}

QMenu *MenuBuilder::Build(model::Node *selectedNode, QWidget *parent)
{
  MenuBuilder builder;
  return builder.BuildPriv(selectedNode, parent);
}

struct MenuTree
{
  std::string name;
  std::vector<MenuTree*> children;

  QMenu* menu;

  MenuTree()
    : menu (nullptr)
  {
    
  }

  ~MenuTree()
  {
    for (auto child : children)
    {
      delete child;
    }
  }

  MenuTree *Get(const std::string &name)
  {
    for (auto child : children)
    {
      if (child->name == name)
      {
        return child;
      }
    }

    MenuTree *newEntry = new MenuTree();
    newEntry->name = name;
    children.push_back(newEntry);
    return newEntry;
  }

  QMenu* build(QMenu *parent = nullptr)
  {
    if (parent)
    {
      menu = parent->addMenu(QString(name.c_str()));
    }
    else
    {
      menu = new QMenu(QString(name.c_str()));
    }
    if (parent)
    {
      parent->addMenu(menu);
    }

    for (auto child : children)
    {
      child->build(menu);
    }

    return menu;
  }
};

QMenu *MenuBuilder::BuildPriv(model::Node *selectedNode, QWidget * parent)
{
  MenuTree *tree = new MenuTree();
  for (auto factory : MenuItemFactories::Get().GetFactories())
  {
    MenuTree *treeEntry = tree;
    for (auto entry : factory->GetMenuItemDeclaration())
    {
      treeEntry = treeEntry->Get(entry.GetMenuName());
    }
  }

  QMenu *menu = tree->build();

  for (auto factory : MenuItemFactories::Get().GetFactories())
  {
    MenuTree *treeEntry = tree;
    for (auto entry : factory->GetMenuItemDeclaration())
    {
      treeEntry = treeEntry->Get(entry.GetMenuName());
    }

    // treeEntry contains the inmost entry within the menu
    factory->CreateAction(treeEntry->menu, selectedNode, parent);
  }



  return menu;
}


}