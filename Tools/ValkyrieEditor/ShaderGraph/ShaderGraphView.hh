#pragma once


#include <qgraphicsview.h>
#include <qwidget.h>
#include <Graph/Scene.hh>
#include <Valkyrie/Types.hh>

class vkSGNode;
class vkSGShaderGraph;
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
  void NodeConnectedLooseInput(graph::Node *inputNode, int inIdx);

private:

  QGraphicsView *m_view;
  graph::NodeGraphScene *m_scene;

  vkSGShaderGraph* m_shaderGraph;

};
