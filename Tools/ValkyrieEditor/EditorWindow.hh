#pragma once

#include <qmainwindow.h>


class vkResourceLocator;
class SceneView;

class EditorView;
class EditorWindow : public QMainWindow
{
  Q_OBJECT;
public:

  EditorWindow();
  virtual ~EditorWindow();


public slots:
  void AddView(EditorView *view);
  void AddView(const vkResourceLocator &resourceLocator);
  void CloseTab(int idx);

private:
  SceneView *m_sceneView;
  QTabWidget *m_mainTabWidget;

  QList<EditorView*> m_views;
};

