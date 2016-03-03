#pragma once

#include <qmainwindow.h>

class ShaderGraphView;

class EditorWindow : public QMainWindow
{
  Q_OBJECT;
public:
  EditorWindow();
  virtual ~EditorWindow();


private:
  ShaderGraphView *m_shaderGraphView;
};