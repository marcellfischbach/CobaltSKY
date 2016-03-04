#pragma once

#include <qopengl.h>
#include <qopenglwidget.h>


class SceneView : public QOpenGLWidget
{
  Q_OBJECT
public:
  SceneView();
  virtual ~SceneView();


protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);

};