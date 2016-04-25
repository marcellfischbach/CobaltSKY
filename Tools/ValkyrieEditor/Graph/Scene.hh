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
  void RemoveSelectedNode();
  void Connect(Node *nodeOutput, int outputIdx, Node *nodeInput, int inputIdx);
  void Disconnect(Node *nodeOutput, int outputIdx, Node *nodeInput, int inputIdx);
  void Disconnect(NodeConnection *nodeConnection, bool resetConst = true);
  void DisconnectInput(Node *nodeInput, int inputIdx);
  void DisconnectOutput(Node *nodeOutput, int outputIdx);

  size_t GetNumberOfNodes();
  Node *GetNode(size_t idx);
  size_t GetNumberOfConnections();
  NodeConnection *GetConnection(size_t idx);

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

  void ResetConstValues();

signals:

  void NodeAdded(graph::Node *node);
  void NodeRemoved(graph::Node *node);
  void NodesConnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx);
  void NodesDisconnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx);
  void NodeConnectedLooseInput(graph::Node *inputNode, int inIdx);
  void NodeConnectedLooseOutput(graph::Node *outputNode, int outIdx);
};

}