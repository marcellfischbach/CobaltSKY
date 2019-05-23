#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <QWidget>
#include <QList>
#include <string>
#include <set>
#include <shadergrapheditor/ui_shadergrapheditorwidget.h>


namespace asset::model
{
  class Entry;
}


namespace cs
{
class ResourceLocator;
class SGNode;
class SGShaderGraph;
}
class ShaderGraphEditor;
class ShaderGraphEditorMeta;
class ShaderGraphEditorNode;
class ShaderGraphEditorWidget : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphEditorWidget(ShaderGraphEditor *parent);
  virtual ~ShaderGraphEditorWidget();

  cs::SGShaderGraph *SetShaderGraph(cs::SGShaderGraph *shaderGraph, ShaderGraphEditorMeta *meta);

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
  void on_pbScreenshot_clicked();

private:
  void QueryResources(cs::SGShaderGraph *graph);
  void SaveCSFFile();
  std::string ExtractName(const cs::ResourceLocator &locator);
  bool Apply();
  void get(QDataStream &stream, std::vector<asset::model::Entry*> &entries) const;
  ShaderGraphEditorNode *GetEditorNode(cs::SGNode *node);
  ShaderGraphEditorNode *GetShaderGraphNode();
  Ui::ShaderGraphEditorWidget m_gui;
  ShaderGraphEditor *m_editor;
  cs::SGShaderGraph *m_shaderGraph;
  cs::SGShaderGraph *m_shaderGraphCopy;

  struct Resource
  {
    std::string name;
    std::string id;
  };

  bool m_updateGuard;

  std::set<std::string> m_resourceIDs;

signals:
  void SelectionChanged(const QList<ShaderGraphEditorNode*> &nodes);
  void ShaderGraphNodeAboutToAdd(cs::SGNode *node);
  void ShaderGraphNodeAdded(cs::SGNode *node);
  void ShaderGraphNodeAboutToRemove(cs::SGNode *node);
  void ShaderGraphNodeRemoved(cs::SGNode *node);
  void ShaderGraphChanged();
};
