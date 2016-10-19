#pragma once


#include <qgraphicsview.h>
#include <qwidget.h>
#include <EditorView.hh>
#include <Graph/Scene.hh>
#include <Valkyrie/Types.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <ui_ShaderGraphView.h>
#include <ShaderGraph/ShaderGraphView.refl.hh>

namespace shadergraph
{
class SGNode;
class SGShaderGraphNode;
class PreviewWidget;
class ShaderGraphScene;
}

class vkSGNode;
class vkSGShaderGraph;
class vkClass;
class ShaderGraphConnection;
class ShaderGraphNode;
class ShaderGraphMetaData;
class ShaderGraphWidget : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphWidget(QWidget *parent = 0);
  virtual ~ShaderGraphWidget();

  void Set(const vkResourceLocator &resourceLocator);
  void Setup(vkSGShaderGraph *shaderGraph, ShaderGraphMetaData *metaData);

  void AddTexture2D(const vkResourceLocator &locator, const QPoint &pos = QPoint(0, 0));

public slots:
  void popupNodeSelector();


protected:
  void keyReleaseEvent(QKeyEvent *event);

private:
  graph::GraphNode *AddNode(vkSGNode *node, const vkVector2f &pos);

  void SyncGraph (const vkSGShaderGraph *src, vkSGShaderGraph *dst);
  void SyncNodes (const vkSGShaderGraph *src, vkSGShaderGraph *dst);
  vkSGNode *Copy (const vkSGNode* node);

private slots:
  void ViewRightClicked(const QPoint&);
  graph::GraphNode *AddNode(const vkClass *clazz);
  void NodeSelected(graph::GraphNode *graphNode);


  void on_pbSave_clicked(bool checked);
  void on_pbCompile_clicked(bool checked);


private:
  bool Compile();

  vkResourceLocator m_resourceLocator;

  Ui::ShaderGraphView m_gui;
  QGraphicsView *m_view;
  shadergraph::ShaderGraphScene *m_scene;

  vkSGShaderGraph* m_shaderGraph;

  shadergraph::PreviewWidget *m_previewWidget;

  vkVector2f m_newNodePosition;

  QWidget *m_editorWidget;


};



class ShaderGraphView : public EditorView
{
public:
  ShaderGraphView(QWidget *parent);
  virtual ~ShaderGraphView();

  void Initialize();
  virtual bool Close();

private:
  ShaderGraphWidget *m_shaderGraphWidget;
};

VK_CLASS()
class ShaderGraphViewFactory : public IEditorViewFactory
{
  VK_CLASS_GEN_OBJECT;
public:
  ShaderGraphViewFactory();
  virtual ~ShaderGraphViewFactory();


  virtual bool CanEdit(const vkResourceLocator &resourceLocator, IObject *obj);
  virtual EditorView *CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj);

};
