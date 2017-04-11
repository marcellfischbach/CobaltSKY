#pragma once


#include <QWidget>

class SceneView : public QWidget
{
public:
  SceneView(QWidget *parent = 0);
  virtual ~SceneView();

  void paintEvent(QPaintEvent *event);
  /*
protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);
  */
};