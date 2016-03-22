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
  void startConnectingInput (const QPointF &pos, int i);
  void startConnectingOutput (const QPointF &pos, int i);
  void processConnecion (const QPointF &pos);

private:
  QGraphicsView *m_view;
  QGraphicsScene *m_scene;


  QGraphicsPathItem *m_currentBounding;
};
