#pragma once


#include <QWidget>

#include <QList>

class NodeGraphNode;
class NodeGraphWidget : public QWidget
{
public:
  NodeGraphWidget(QWidget *parent = 0);
  ~NodeGraphWidget();

  void AddNode(NodeGraphNode *node);
  void RemoveNode(NodeGraphNode *node);

  void paintEvent(QPaintEvent *event);

private:

  QList<NodeGraphNode*> m_nodes;
};