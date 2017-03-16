#pragma once

#include <QWidget>
#include <QList>
#include <ui_shadergrapheditorwidget.h>

class vkSGNode;
class vkSGShaderGraph;
class ShaderGraphEditor;
class ShaderGraphEditorMeta;
class ShaderGraphEditorNode;
class ShaderGraphEditorWidget : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphEditorWidget(ShaderGraphEditor *parent);
  virtual ~ShaderGraphEditorWidget();

  void SetShaderGraph(vkSGShaderGraph *shaderGraph, ShaderGraphEditorMeta *meta);

public slots:
void RepaintGraph();

private slots:
  void on_nodeGraph_CheckConnection(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB, NodeGraphVetoEvent *veto);
  void on_nodeGraph_AboutToConnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void on_nodeGraph_Connected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void on_nodeGraph_Disconnected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void on_nodeGraph_CheckLooseDisconnect(NodeGraphNodeAnchor *anchor, NodeGraphVetoEvent *veto);
  void on_nodeGraph_ScaleChanged(float scale);
  void on_nodeGraph_CheckDrag(const QDropEvent *event, NodeGraphAcceptEvent *accept);
  void on_nodeGraph_DragDropped(const QDropEvent *event);
  void on_nodeGraph_RequestRemoveNode(QList<NodeGraphNode*> nodes, NodeGraphVetoEvent *veto);
  void on_nodeGraph_NodeRemoved(NodeGraphNode* node);
  void on_nodeGraph_SelectionChanged(const QList<NodeGraphNode*> &nodes);

  void on_pbApply_clicked();
  void on_pbSave_clicked();

private:
  bool Apply();
  ShaderGraphEditorNode *GetEditorNode(vkSGNode *node);
  ShaderGraphEditorNode *GetShaderGraphNode();
  Ui::ShaderGraphEditorWidget m_gui;
  ShaderGraphEditor *m_editor;
  vkSGShaderGraph *m_shaderGraph;
  vkSGShaderGraph *m_shaderGraphCopy;

  bool m_updateGuard;

signals:
  void SelectionChanged(const QList<ShaderGraphEditorNode*> &nodes);
};
