#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <QWidget>

#include <shadergrapheditor/ui_shadergrapheditortoolbox.h>

class ShaderGraphEditorToolboxModel;
class ShaderGraphEditorToolbox : public QWidget
{
  Q_OBJECT;
public:
  ShaderGraphEditorToolbox();
  virtual ~ShaderGraphEditorToolbox();

private slots:
void on_leFilter_textChanged(const QString &filter);
private:
  Ui::ShaderGraphEditorToolbox m_gui;
  ShaderGraphEditorToolboxModel *m_model;
};