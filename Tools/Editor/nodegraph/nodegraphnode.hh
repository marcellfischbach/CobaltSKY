#pragma once

#include <QPainter>
#include <QRect>

class NodeGraphNodeHeader;
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

  NodeGraphNodeHeader* GetHeader()
  {
    return m_header;
  }


private:
  QRectF m_bounding;
  NodeGraphNodeHeader *m_header;
};