#pragma once


#include <QWidget>

#include <QList>

class NodeGraphNode;
class NodeGraphNodeAnchor;
class NodeGraphWidget : public QWidget
{
public:
  NodeGraphWidget(QWidget *parent = 0);
  ~NodeGraphWidget();

  void AddNode(NodeGraphNode *node);
  void RemoveNode(NodeGraphNode *node);

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);


private:
  QPointF map(const QPoint &p) const;
  NodeGraphNode *m_dragNode;
  NodeGraphNode *m_dragAnchorNode;
  NodeGraphNodeAnchor *m_dragAnchor;
  QList<NodeGraphNode*> m_nodes;
  QPointF m_dragPos;
};