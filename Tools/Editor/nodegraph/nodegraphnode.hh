#pragma once

#include <QPainter>
#include <QRect>

class NodeGraphHeader;
class NodeGraphNode
{
public:
  NodeGraphNode();
  ~NodeGraphNode();

  void Layout();

  void paint(QPainter *painter);

  void SetLocation(float x, float y);

  void SetBounding(const QRectF &bounding)
  {
    m_bounding = bounding;
  }

  const QRectF &GetBounding() const
  {
    return m_bounding;
  }

  NodeGraphHeader* GetHeader()
  {
    return m_header;
  }


private:
  QRectF m_bounding;
  NodeGraphHeader *m_header;
};