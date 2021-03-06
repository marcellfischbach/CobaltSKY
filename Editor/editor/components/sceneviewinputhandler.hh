#pragma once

#include <editor/editorexport.hh>

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
struct EDITOR_API SceneViewInputHandler
{
  virtual ~SceneViewInputHandler () { }

  virtual void mousePressEvent(QMouseEvent *event) = 0;
  virtual void mouseReleaseEvent(QMouseEvent *event) = 0;
  virtual void mouseMoveEvent(QMouseEvent *event) = 0;

  virtual void wheelEvent(QWheelEvent *event) = 0;

  virtual void keyPressEvent(QKeyEvent *event) = 0;
  virtual void keyReleaseEvent(QKeyEvent *event) = 0;

  virtual void timedUpdate(float deltaT) = 0;

};