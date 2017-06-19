#pragma once

#include <QWidget>
#include <ui_shadergrapheditoroutliner.h>

class ShaderGraphEditorNode;
class csSGShaderGraph;
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

private:
  Ui::ShaderGraphEditorOutliner m_gui;
  ShaderGraphEditorOutlinerTableModel *m_model;

};