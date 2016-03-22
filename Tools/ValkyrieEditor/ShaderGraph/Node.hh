
#pragma once

#include <qgraphicsitem.h>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>


class ShaderGraphNode : public QObject
{
  Q_OBJECT
public:
  ShaderGraphNode (vkSGNode *node);

  QGraphicsItem *GetItem ();

private:
  QGraphicsItemGroup *m_parent;
  float m_titleFontSize;
  float m_inoutFontSize;
  QGraphicsRectItem *m_background;
  QGraphicsSimpleTextItem *m_title;

  struct Anchor
  {
    QGraphicsEllipseItem *anchor;
    QGraphicsSimpleTextItem *text;
    QGraphicsTextItem *editValue;
    QGraphicsRectItem *editValueBackground;
  };

  QList<Anchor> m_inputs;
  QList<Anchor> m_outputs;

  vkSGNode *m_node;
};

class ShaderGraphNode2 : public QGraphicsItem
{
public:
  ShaderGraphNode2();

  bool Initialize ();

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

  virtual QRectF boundingRect() const;

  const vkSGNode *GetNode() const;
  void SetNode(vkSGNode *node);

  static void SetSelectedNode (ShaderGraphNode2 *selectedNode);
  static ShaderGraphNode2 *GetSelectedNode ();

protected:
  void	mousePressEvent(QGraphicsSceneMouseEvent *event);
  void	mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void	mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
  vkSGNode *m_node;
  QRectF m_size;
  QRectF m_handleSize;
  QPointF m_scenePos;
  QPointF m_pos;
  bool m_inMotion;
  bool m_connecting;

  int m_fontSize;
  int m_inOutStart;
  int m_margin;
  int m_spacing;

  QList<QRectF> m_inputRects;
  QList<QRectF> m_outputRects;

  static ShaderGraphNode2 *selectedNode;
  /*
signals:
  void startConnectionInput(const QPointF &point, int i);
  void startConnectionOutput(const QPointF &point, int i);
  void processConnection (const QPointF &point);
  */
};

