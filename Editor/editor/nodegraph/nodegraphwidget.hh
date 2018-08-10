#pragma once


#include <editor/editorexport.hh>

#include <QList>
#include <QMimeData>
#include <QWidget>

#include <editor/nodegraph/nodegraphacceptevent.hh>
#include <editor/nodegraph/nodegraphvetoevent.hh>


class NodeGraphNode;
class NodeGraphNodeAnchor;
class EDITOR_API NodeGraphWidget : public QWidget
{
  Q_OBJECT

public:
  struct Conn
  {
    NodeGraphNode *outputNode;
    unsigned outputNodeOutputIdx;

    NodeGraphNode *inputNode;
    unsigned inputNodeInputIdx;
  };
public:
  NodeGraphWidget(QWidget *parent = 0);
  ~NodeGraphWidget();

  void Clear();

  void AddNode(NodeGraphNode *node);
  void RemoveNode(NodeGraphNode *node);

  size_t GetNumberOfNodes() const;
  NodeGraphNode *GetNode(size_t idx) const;

  size_t GetNumberOfConnections() const;
  const Conn GetConnection(size_t idx) const;

  void Connect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void Disconnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void DisconnectAll(NodeGraphNodeAnchor *anchor);

  void ClearSelection(bool emitSignal = true);

  QPointF GetLocalCoordinate(const QPoint &screenCoord);
protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);
  void paintEvent(QPaintEvent *event);
  void dragEnterEvent(QDragEnterEvent *event);
  void dragMoveEvent(QDragMoveEvent *event);
  void dropEvent(QDropEvent *event);

  NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const;

signals:
  void CheckLooseDisconnect(NodeGraphNodeAnchor *anchor, NodeGraphVetoEvent *veto);
  void CheckConnection(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB, NodeGraphVetoEvent *veto);
  void AboutToConnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void Connected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void RequestRemoveNode(QList<NodeGraphNode*> node, NodeGraphVetoEvent *veto);
  void NodeRemoved(NodeGraphNode *node);

  void AboutToDisconnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void Disconnected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void ScaleChanged(float scale);

  void CheckDrag(const QDropEvent *event, NodeGraphAcceptEvent *accept);
  void DragDropped(const QDropEvent *event);

  void SelectionChanged(const QList<NodeGraphNode*> &nodes);
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