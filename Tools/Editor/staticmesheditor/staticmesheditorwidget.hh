#pragma once

#include <QWidget>

#include <ui_staticmesheditorwidget.h>

class csStaticMeshState;
class StaticMeshEditor;
class StaticMeshEditorWidget : public QWidget
{
  Q_OBJECT
public:
  StaticMeshEditorWidget(StaticMeshEditor *editor);
  virtual ~StaticMeshEditorWidget();

  void SetStaticMeshState(csStaticMeshState *staticMeshState);

private:
  Ui::StaticMeshEditorWidget m_gui;

  StaticMeshEditor *m_editor;
};