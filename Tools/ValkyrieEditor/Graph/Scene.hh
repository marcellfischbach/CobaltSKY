#pragma once

#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qlist.h>
#include <Graph/Direction.h>

class Node;
class NodeConnection;
class NodeGraphScene : public QGraphicsScene
{
public:
  NodeGraphScene(QObject *parent = 0);

  void AddNode(Node *node);
  void Connect(Node *nodeOutput, int outputIdx, Node *nodeInput, int inputIdx);

public:
  void NodeMoved(Node *node);
  void MoveConnection(Node *node, int idx, Direction dir, const QPointF &p0, const QPointF &p1);
  void StopConnection(Node *node, int idx, Direction dir, const QPointF &p0, const QPointF &p1);

private:
  QPointF TestForAnchorMatch(const QPointF &p, Node *node, Direction dir);
  QGraphicsPathItem *GetCurrentConnectionPath();
  QList<Node*> m_nodes;
  QList<NodeConnection*> m_connections;
  QGraphicsPathItem *m_currentConnectionPath;
};