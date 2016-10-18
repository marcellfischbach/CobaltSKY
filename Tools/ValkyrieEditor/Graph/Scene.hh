#pragma once

#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qlist.h>

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

  void AddNode(GraphNode *node);
  void RemoveNode(GraphNode *node);
  void RemoveSelectedNodes();

  size_t GetNumberOfNodes();
  GraphNode *GetNode(size_t idx);
  size_t GetNumberOfConnections();
  AnchorConnectionItem *GetConnection(size_t idx);

  void Clear ();

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

  virtual void OnNodeAdded (GraphNode *node);
  virtual void OnNodeRemoved(GraphNode *node);

  virtual bool IsConnectable(AnchorWidget *anchorA, AnchorWidget *anchorB) const;
  virtual void OnConnectionAdded (AnchorConnectionItem *connection);
  virtual void OnConnectionCanceled (AnchorWidget *anchor);
  virtual void OnConnectionRemoved(AnchorConnectionItem *connection);

private:
  AnchorWidget *FindAnchorWidget(const QPointF& scenePos);
  QGraphicsPathItem *GetCurrentConnectionPath();

  QList<GraphNode*> m_graphNodes;
  QGraphicsPathItem *m_currentConnectionPath;
  AnchorWidget *m_currentAnchorWidget;
  QList<AnchorConnectionItem*> m_connectionItems;


signals:
  void NodeSelected(graph::GraphNode *node);
  void NodeAdded(graph::GraphNode *node);
  void NodeRemoved(graph::GraphNode *node);

  void ConnectionAdded (AnchorConnectionItem *anchor);
  void ConnectionRemoved (AnchorConnectionItem *anchor);
};

}
