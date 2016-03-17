
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

private:
  vkSGNode *m_node;
};

