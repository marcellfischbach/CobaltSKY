

#pragma once

#include <QAction>
#include <QObject>
#include <QWidget>

namespace cs::editor::model
{
class Node;
class Model;
}

namespace cs::editor::assetmanager
{

class MenuItemHandler : public QObject
{
  Q_OBJECT
public:
  virtual ~MenuItemHandler();

  QAction *GetAction() const;
  model::Node *GetSelectedNode() const;
  model::Model *GetModel() const;
  QWidget *GetParentWidget() const;

protected:
  MenuItemHandler(QAction *action, model::Node *selectedNode, QWidget * parent);

  virtual void OnActionTriggered(bool selected) = 0;

private slots:
  void ActionTriggered(bool selected);
  void Destroyed(QObject *);

private:
  QAction * m_action;
  model::Node *m_selectedNode;
  QWidget *m_parentWidget;
};

}