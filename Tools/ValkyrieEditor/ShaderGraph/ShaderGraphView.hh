#pragma once


#include <qgraphicsview.h>
#include <qwidget.h>

class ShaderGraphView : public QWidget
{
public:
  ShaderGraphView(QWidget *parent = 0);
  virtual ~ShaderGraphView();

private:
  QGraphicsView *m_view;
  QGraphicsScene *m_scene;
};