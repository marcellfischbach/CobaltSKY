#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <QWidget>
#include <shadergrapheditor/ui_shadergrapheditoroutliner.h>

class ShaderGraphEditorNode;
class ShaderGraphEditorOutlinerTableModel;

namespace cs
{
class SGShaderGraph;
class SGNode;
}

class ShaderGraphEditorOutliner : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphEditorOutliner(QWidget *parent = 0);
  virtual ~ShaderGraphEditorOutliner();

  void SetShaderGraph(cs::SGShaderGraph *shaderGraph);

  public slots:
  void SetSelectedNodes(const QList<ShaderGraphEditorNode*>& nodes);
  void NodeChanged(ShaderGraphEditorNode* node);
  void NodeAboutToAdd(cs::SGNode *node);
  void NodeAdded(cs::SGNode *node);
  void NodeAboutToRemove(cs::SGNode *node);
  void NodeRemoved(cs::SGNode *node);

private:
  Ui::ShaderGraphEditorOutliner m_gui;
  ShaderGraphEditorOutlinerTableModel *m_model;

};