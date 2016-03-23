#pragma once


#include <qgraphicsview.h>
#include <qwidget.h>

class vkClass;
class ShaderGraphConnection;
class ShaderGraphNode;
class ShaderGraphView : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphView(QWidget *parent = 0);
  virtual ~ShaderGraphView();

public slots:
  void popupNodeSelector ();


protected:
  void keyReleaseEvent (QKeyEvent *event);


private slots:
  void addNode (const vkClass *clazz);
  void moveDrag (bool input, int idx, const QPointF &pointA, const QPointF &pointB);
  void stopDrag (bool input, int idx, const QPointF& pointA, const QPointF & pointB);

private:
  QPointF testAnchor (ShaderGraphNode *node, bool input, const QPointF &p);
  bool testAnchor (ShaderGraphNode *node, bool input, const QPointF &p, ShaderGraphNode **other, int *index);

private:
  QGraphicsView *m_view;
  QGraphicsScene *m_scene;


  QGraphicsPathItem *m_currentBounding;

  QList<ShaderGraphNode*> m_nodes;
};
