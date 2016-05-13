#pragma once

#include <qevent.h>

namespace scenewidget
{


class EventListener
{
public:
  virtual bool keyPressEvent(QKeyEvent *event) { return false; }
  virtual bool keyReleaseEvent(QKeyEvent *event) { return false; }
  virtual bool mouseMoveEvent(QMouseEvent *event) { return false; }
  virtual bool mousePressEvent(QMouseEvent *event) { return false; }
  virtual bool mouseReleaseEvent(QMouseEvent *event) { return false; }
  virtual bool wheelEvent(QWheelEvent *event) { return false; }
};


}