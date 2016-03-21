#pragma once


#include <qgraphicsview.h>
#include <qwidget.h>

class vkClass;

class ShaderGraphView : public QWidget
{
  Q_OBJECT
public:
  ShaderGraphView(QWidget *parent = 0);
  virtual ~ShaderGraphView();

public slots:
  void popupNodeSelector ();


protected:
  void keyReleaseEvent (QKeyEvent *event);


private slots:
  void addNode (const vkClass *clazz);

private:
  QGraphicsView *m_view;
  QGraphicsScene *m_scene;
};
