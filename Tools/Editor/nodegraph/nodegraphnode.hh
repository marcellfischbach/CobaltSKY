#pragma once

#include <QPainter>
#include <QRect>

class NodeGraphNode
{
public:
  NodeGraphNode();
  ~NodeGraphNode();


  void paint(QPainter *painter);

  const QRectF &GetBounding() const
  {
    return m_bounding;
  }

private:
  QRectF m_bounding;
};