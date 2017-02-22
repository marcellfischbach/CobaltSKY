#pragma once

#include <QWidget>
#include <QList>
#include <ui_shadergrapheditorwidget.h>

class ShaderGraphEditor;
class ShaderGraphEditorNode;
class ShaderGraphEditorWidget : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphEditorWidget(ShaderGraphEditor *parent);
  virtual ~ShaderGraphEditorWidget();

public slots:
void RepaintGraph();

private slots:
  void on_nodeGraph_CheckConnection(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB, NodeGraphVetoEvent *veto);
  void on_nodeGraph_AboutToConnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB);
  void on_nodeGraph_CheckLooseDisconnect(NodeGraphNodeAnchor *anchor, NodeGraphVetoEvent *veto);
  void on_nodeGraph_ScaleChanged(float scale);
  void on_nodeGraph_CheckDrag(const QDropEvent *event, NodeGraphAcceptEvent *accept);
  void on_nodeGraph_DragDropped(const QDropEvent *event);
  void on_nodeGraph_RequestRemoveNode(QList<NodeGraphNode*> nodes, NodeGraphVetoEvent *veto);
  void on_nodeGraph_SelectionChanged(const QList<NodeGraphNode*> &nodes);
private:
  Ui::ShaderGraphEditorWidget m_gui;
  ShaderGraphEditor *m_editor;

signals:
  void SelectionChanged(const QList<ShaderGraphEditorNode*> &nodes);
};
