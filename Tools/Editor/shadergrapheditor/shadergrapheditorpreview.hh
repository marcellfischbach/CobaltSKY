#pragma once

#include <QWidget>

#include <ui_shadergrapheditorpreview.h>

class csSGShaderGraph;
class ShaderGraphEditorPreview : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphEditorPreview(QWidget *parent = 0);
  virtual ~ShaderGraphEditorPreview();

  void SetShaderGraph(csSGShaderGraph *shaderGraph);

public slots:
void ShaderGraphChanged();
  

private:
  Ui::ShaderGraphEditorPreview m_gui;
};