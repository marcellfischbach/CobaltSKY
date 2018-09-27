#pragma once

#include <editor/editorexport.hh>

class QMouseEvent;
class QWheelEvent;
struct EDITOR_API PreviewInputHandler
{
  virtual ~PreviewInputHandler() { }

  virtual void mousePressEvent(QMouseEvent *event) = 0;
  virtual void mouseReleaseEvent(QMouseEvent *event) = 0;
  virtual void mouseMoveEvent(QMouseEvent *event) = 0;

  virtual void wheelEvent(QWheelEvent *event) = 0;
};