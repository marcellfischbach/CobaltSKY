#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <QWidget>

#include <shadergrapheditor/ui_shadergrapheditorpreview.h>

class csSGShaderGraph;
class ShaderGraphEditorPreview : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphEditorPreview(QWidget *parent = 0);
  virtual ~ShaderGraphEditorPreview();

  void SetShaderGraph(csSGShaderGraph *shaderGraph);
  QImage TakeScreenshot(unsigned width, unsigned height);

public slots:
void ShaderGraphChanged();
  

private:
  Ui::ShaderGraphEditorPreview m_gui;
};