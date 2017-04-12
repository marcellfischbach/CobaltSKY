#pragma once

#include <QWidget>

#include <ui_shadergrapheditorpreview.h>

class ShaderGraphEditorPreview : public QWidget
{
public:
  ShaderGraphEditorPreview(QWidget *parent = 0);
  virtual ~ShaderGraphEditorPreview();

private:
  Ui::ShaderGraphEditorPreview m_gui;
};