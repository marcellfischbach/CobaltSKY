#pragma once

#include <qmainwindow.h>


class ShaderGraphView;
class SceneView;

class EditorWindow : public QMainWindow
{
  Q_OBJECT;
public:
  struct View
  {
    QString name;
    QWidget *view;
    View(const QString &name, QWidget *view) 
      : name(name)
      , view(view)
    {

    }
  };

public:
  EditorWindow();
  virtual ~EditorWindow();

  void AddView(View view);

private:
  SceneView *m_sceneView;
  ShaderGraphView *m_shaderGraphView;
  QTabWidget *m_mainTabWidget;
};
