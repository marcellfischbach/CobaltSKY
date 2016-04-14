#pragma once

#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qlist.h>
#include <Graph/Direction.h>

namespace graph
{

class Node;
class NodeConnection;
class NodeGraphScene : public QGraphicsScene
{
  Q_OBJECT
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


signals:

  void NodeAdded(Node *node);
  void NodeRemoved(Node *node);
  void NodesConnected(Node *outNode, int outIdx, Node *inNode, int inIdx);
  void NodesDisconnected(Node *outNode, int outIdx, Node *inNode, int inIdx);
};

}