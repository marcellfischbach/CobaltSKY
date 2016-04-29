#pragma once


#include <qgraphicsview.h>
#include <qwidget.h>
#include <Graph/Scene.hh>
#include <Valkyrie/Types.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <ui_ShaderGraphView.h>

namespace shadergraph
{
class SGNode;
class SGShaderGraphNode;
class ResourcesModel;
class PreviewWidget;
}

class vkSGNode;
class vkSGShaderGraph;
class vkClass;
class ShaderGraphConnection;
class ShaderGraphNode;
class ShaderGraphMetaData;
class ShaderGraphView : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphView(QWidget *parent = 0);
  virtual ~ShaderGraphView();

  void Set(const vkResourceLocator &resourceLocator);
  void Setup(vkSGShaderGraph *shaderGraph, ShaderGraphMetaData *metaData);

  public slots:
  void popupNodeSelector();


protected:
  void keyReleaseEvent(QKeyEvent *event);


private:
  graph::Node *AddNode(const vkSGNode *node, const vkVector2f &pos);
  graph::Node *AddNode(const vkClass *clazz, const vkSGNode *node, const vkVector2f &pos);

private slots:
  graph::Node *AddNode(const vkClass *clazz);
  void NodeNameChanged(graph::Node *node);
  void NodeRemoved(graph::Node* node);
  void NodeConnectedLooseInput(graph::Node *inputNode, int inIdx);
  void ResourceDoubleClicked(const QModelIndex &index);
  void ResourceEditApplied(shadergraph::SGNode *node);


  void on_cbDiscardAlpha_stateChanged(int state);
  void on_pbSave_clicked(bool checked);
  void on_pbCompile_clicked(bool checked);


private:
  bool Compile();
  void CollectData(vkSGShaderGraph *graph, std::map<graph::Node*, vkSGNode*>& nodes);

  vkResourceLocator m_resourceLocator;

  Ui::ShaderGraphView m_gui;
  QGraphicsView *m_view;
  graph::NodeGraphScene *m_scene;

  vkSGShaderGraph* m_shaderGraph;
  shadergraph::SGShaderGraphNode *m_shaderGraphNode;

  shadergraph::ResourcesModel *m_resourcesModel;

  shadergraph::PreviewWidget *m_previewWidget;

};
