
#pragma once

#include <qgraphicsitem.h>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>

class ShaderGraphNode : public QGraphicsItem
{
public:
  ShaderGraphNode();

  bool Initialize ();

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

  virtual QRectF boundingRect() const;

  const vkSGNode *GetNode() const;
  void SetNode(vkSGNode *node);

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

  int m_fontSize;
  int m_inOutStart;
  int m_margin;
  int m_spacing;
};

