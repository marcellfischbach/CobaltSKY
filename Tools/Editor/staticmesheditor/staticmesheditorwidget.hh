#pragma once

#include <QWidget>

#include <ui_staticmesheditorwidget.h>

class StaticMeshEditor;
class StaticMeshEditorWidget : public QWidget
{
  Q_OBJECT
public:
  StaticMeshEditorWidget(StaticMeshEditor *editor);
  virtual ~StaticMeshEditorWidget();

private:
  Ui::StaticMeshEditorWidget m_gui;

  StaticMeshEditor *m_editor;
};