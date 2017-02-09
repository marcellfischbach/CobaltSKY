#pragma once


#include <QWidget>

#include <QList>
#include <nodegraph/nodegraphvetoevent.hh>


class NodeGraphNode;
class NodeGraphNodeAnchor;
class NodeGraphWidget : public QWidget
{
  Q_OBJECT
public:
  NodeGraphWidget(QWidget *parent = 0);
  ~NodeGraphWidget();

  void AddNode(NodeGraphNode *node);
  void RemoveNode(NodeGraphNode *node);

  void Connect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void Disconnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void DisconnectAll(NodeGraphNodeAnchor *anchor);

  void ClearSelection();

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void paintEvent(QPaintEvent *event);

  NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const;

signals:
  void CheckLooseDisconnect(NodeGraphNodeAnchor *anchor, NodeGraphVetoEvent *veto);
  void CheckConnection(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB, NodeGraphVetoEvent *veto);
  void AboutToConnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void Connected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);

  void AboutToDisconnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void Disconnected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void ScaleChanged(float scale);

private:
  struct Connection
  {
    NodeGraphNodeAnchor *m_anchorA;
    NodeGraphNodeAnchor *m_anchorB;
    bool operator== (const Connection &other) const;
  };

  void SortNodesBySelection();
  void UpdateAnchorConnectionState();
  QTransform GetTransform() const;

  QPointF map(const QPoint &p) const;
  QPoint unmap(const QPointF &p) const;
  bool AddSelection(QMouseEvent *event) const;
  void SelectByRubberBand();

  bool m_moveSelectedNodes;
  QList<NodeGraphNode*> m_selectedNodes;
  NodeGraphNodeAnchor *m_dragAnchor;
  QList<NodeGraphNode*> m_nodes;
  QList<Connection> m_connections;
  QPointF m_dragStart;
  QPointF m_dragPos;
  bool m_rubberBand;
  bool m_armTranslation;

  float m_scale;
  QPointF m_translate;
  QPointF m_translateOffset;
  QPoint m_transStart;
  QPoint m_transPos;
};