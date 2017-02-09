#pragma once

#include <QRectF>
#include <nodegraph/nodegraphnodepropertytype.hh>

class NodeGraphNode;
class NodeGraphNodeAnchor
{
public:
  NodeGraphNodeAnchor(NodeGraphNode *node);

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

  NodeGraphNode *GetNode() const
  {
    return m_node;
  }

  void SetConnected(bool connected)
  {
    m_connected = connected;
  }

  bool IsConnected() const
  {
    return m_connected;
  }

  QPointF GetCenter() const;
  QPointF GetAbsCenter() const;

  

  bool Test(const QPointF &point) const;

private:
  NodeGraphNode *m_node;
  QRectF m_bounds;
  bool m_connected;

  NodeGraphNodePropertyType m_type;

};