#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <QWidget>
#include <shadergrapheditor/ui_shadergrapheditoroutliner.h>

class ShaderGraphEditorNode;
class csSGShaderGraph;
class csSGNode;
class ShaderGraphEditorOutlinerTableModel;
class ShaderGraphEditorOutliner : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphEditorOutliner(QWidget *parent = 0);
  virtual ~ShaderGraphEditorOutliner();

  void SetShaderGraph(csSGShaderGraph *shaderGraph);

  public slots:
  void SetSelectedNodes(const QList<ShaderGraphEditorNode*>& nodes);
  void NodeChanged(ShaderGraphEditorNode* node);
  void NodeAboutToAdd(csSGNode *node);
  void NodeAdded(csSGNode *node);
  void NodeAboutToRemove(csSGNode *node);
  void NodeRemoved(csSGNode *node);

private:
  Ui::ShaderGraphEditorOutliner m_gui;
  ShaderGraphEditorOutlinerTableModel *m_model;

};