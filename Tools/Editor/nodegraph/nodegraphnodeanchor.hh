#pragma once

#include <QRectF>
#include <nodegraph/nodegraphnodepropertytype.hh>

class NodeGraphNodeAnchor
{
public:
  NodeGraphNodeAnchor();

  void SetBounds(const QRectF &bounds)
  {
    m_bounds = bounds;
  }
  const QRectF &GetBounds() const
  {
    return m_bounds;
  }

  void SetType(NodeGraphNodePropertyType type)
  {
    m_type = type;
  }

  NodeGraphNodePropertyType GetType() const
  {
    return m_type;
  }

  bool Test(const QPointF &point) const;

private:

  QRectF m_bounds;

  NodeGraphNodePropertyType m_type;

};