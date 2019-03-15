
#include <assetmanager/menu/menuitemhandler.hh>
#include <editormodel/nodes/node.hh>

#include <iostream>

namespace cs::editor::assetmanager
{

MenuItemHandler::MenuItemHandler(QAction *action, model::Node *selectedNode, QWidget * parent)
  : QObject(parent)
  , m_action(action)
  , m_selectedNode(selectedNode)
  , m_parentWidget(parent)
{
  connect(action, SIGNAL(triggered(bool)), this, SLOT(ActionTriggered(bool)));
  connect(action, SIGNAL(destroyed(QObject*)), this, SLOT(Destroyed(QObject*)));
}

MenuItemHandler::~MenuItemHandler()
{
  std::cout << "MenuItemHandler::~MenuItemHandler\n";
}

QAction * MenuItemHandler::GetAction() const
{
  return m_action;
}

model::Node *MenuItemHandler::GetSelectedNode() const
{
  return m_selectedNode;
}

model::Model *MenuItemHandler::GetModel() const
{
  return m_selectedNode->GetModel();
}

QWidget * MenuItemHandler::GetParentWidget() const
{
  return m_parentWidget;
}

void MenuItemHandler::ActionTriggered(bool selected)
{
  OnActionTriggered(selected);
}

void MenuItemHandler::Destroyed(QObject *parent)
{
  deleteLater();
}
}