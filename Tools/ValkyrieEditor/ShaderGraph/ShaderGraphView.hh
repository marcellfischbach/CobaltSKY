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
  graph::Node *AddNode(const vkClass *clazz);
  void NodeAdded(graph::Node *node);
  void NodeRemoved(graph::Node *node);
  void NodesConnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx);
  void NodesDisconnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx);
  void NodeConnectedLooseInput(graph::Node *inputNode, int inIdx);

private:
  void RemoveAllConnections(vkSGNode *node);

  QGraphicsView *m_view;
  graph::NodeGraphScene *m_scene;

  QMap<graph::Node*, vkSGNode*> m_nodes;
  QMap<vkSGNode*, graph::Node*> m_nodesRev;
};
