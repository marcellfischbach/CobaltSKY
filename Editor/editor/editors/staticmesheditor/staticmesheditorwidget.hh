#pragma once

#include <editor/editorexport.hh>
#include <QWidget>

#include <editor/ui_staticmesheditorwidget.h>

namespace cs
{
class StaticMeshState;
}

class StaticMeshEditor;
class StaticMeshEditorWidget : public QWidget
{
  Q_OBJECT
public:
  StaticMeshEditorWidget(StaticMeshEditor *editor);
  virtual ~StaticMeshEditorWidget();

  void SetStaticMeshState(cs::StaticMeshState *staticMeshState);

private:
  Ui::StaticMeshEditorWidget m_gui;

  StaticMeshEditor *m_editor;
};