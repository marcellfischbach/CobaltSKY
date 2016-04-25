#pragma once


#include <qgraphicsview.h>
#include <qwidget.h>
#include <Graph/Scene.hh>
#include <Valkyrie/Types.hh>
#include <ui_ShaderGraphView.h>

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

  void Setup(vkSGShaderGraph *shaderGraph);

  public slots:
  void popupNodeSelector();


protected:
  void keyReleaseEvent(QKeyEvent *event);


  private slots:
  graph::Node *AddNode(const vkClass *clazz);
  void NodeConnectedLooseInput(graph::Node *inputNode, int inIdx);

  void on_cbDiscardAlpha_stateChanged(int state);
  void on_pbSave_clicked(bool checked);
  void on_pbCompile_clicked(bool checked);


private:

  Ui::ShaderGraphView m_gui;
  QGraphicsView *m_view;
  graph::NodeGraphScene *m_scene;

  vkSGShaderGraph* m_shaderGraph;

};
