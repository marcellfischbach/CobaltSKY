#pragma once


#include <qgraphicsview.h>
#include <qwidget.h>
#include <Graph/Scene.hh>

class vkSGNode;
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
  void popupNodeSelector();


protected:
  void keyReleaseEvent(QKeyEvent *event);


  private slots:
  void AddNode(const vkClass *clazz);
  void NodeAdded(graph::Node *node);
  void NodeRemoved(graph::Node *node);
  void NodesConnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx);
  void NodesDisnnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx);

private:
  QGraphicsView *m_view;
  graph::NodeGraphScene *m_scene;

  QMap<graph::Node*, vkSGNode*> m_nodes;
};
