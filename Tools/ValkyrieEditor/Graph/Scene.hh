#pragma once

#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qlist.h>
#include <Graph/Direction.h>

namespace graph
{

class GraphNode;
class Node;
class NodeConnection;
class AttribAnchorWidget;
class AnchorWidget;
class AnchorConnectionItem;
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

  void SelectNode (graph::Node *node);
  graph::Node *GetSelectedNode ();
  const graph::Node *GetSelectedNode () const;

  void SetSilent(bool silent);
  bool IsSilent () const;

public:
  void NodeMoved(Node *node);
  void MoveConnection(Node *node, int idx, Direction dir, const QPointF &p0, const QPointF &p1);
  void StopConnection(Node *node, int idx, Direction dir, const QPointF &p0, const QPointF &p1);



  void StartConnection(AnchorWidget *item);
  void MoveConnection(const QPointF &scenePos);
  void AttachConnect (const QPointF &scenePos);
  AnchorConnectionItem *AddConnection (AnchorWidget *anchorA, AnchorWidget *anchorB);
  bool RemoveConnection (AnchorWidget *anchorA, AnchorWidget *anchorB);
  bool RemoveConnection (AnchorConnectionItem *connection);
  void ClearSelection ();
  void SelectNode (GraphNode *node, Qt::KeyboardModifiers modifiers);

  void StartMoving ();
  void MoveSelectedNodes (const QPointF &direction);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

  virtual bool IsConnectable(AnchorWidget *anchorA, AnchorWidget *anchorB) const;
  virtual void OnConnectionAdded (AnchorConnectionItem *connection) { };
  virtual void OnConnectionCanceled (AnchorWidget *anchor) { };
  virtual void OnConnectionRemoved(AnchorConnectionItem *connection) { };

private:
  AnchorWidget *FindAnchorWidget(const QPointF& scenePos);
  QPointF TestForAnchorMatch(const QPointF &p, Node *node, Direction dir);
  QGraphicsPathItem *GetCurrentConnectionPath();
  QList<Node*> m_nodes;
  QList<NodeConnection*> m_connections;
  graph::Node *m_currentSelectedNode;


  QList<GraphNode*> m_graphNodes;
  QGraphicsPathItem *m_currentConnectionPath;
  AnchorWidget *m_currentAnchorWidget;
  QList<AnchorConnectionItem*> m_connectionItems;

  bool m_silent;

  void ResetConstValues();
  void EmitCurrentNodeChanged ();


signals:
  void NodeSelected(graph::Node *node);
  void NodeAdded(graph::Node *node);
  void NodeRemoved(graph::Node *node);
  void NodeNameChanged(graph::Node *node);
  void NodesConnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx);
  void NodesDisconnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx);
  void NodeConnectedLooseInput(graph::Node *inputNode, int inIdx);
  void NodeConnectedLooseOutput(graph::Node *outputNode, int outIdx);
};

}
