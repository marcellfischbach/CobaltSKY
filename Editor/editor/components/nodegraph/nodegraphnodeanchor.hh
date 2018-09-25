#pragma once

#include <editor/editorexport.hh>
#include <QRectF>
#include <editor/components/nodegraph/nodegraphnodepropertytype.hh>

class NodeGraphNode;
class NodeGraphNodeProperty;
class EDITOR_API NodeGraphNodeAnchor
{
public:
  NodeGraphNodeAnchor(NodeGraphNode *node, NodeGraphNodeProperty *prop);

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

  NodeGraphNodeProperty *GetProperty() const
  {
    return m_prop;
  }

  bool Test(const QPointF &point) const;

private:
  NodeGraphNode *m_node;
  QRectF m_bounds;
  bool m_connected;

  NodeGraphNodePropertyType m_type;
  NodeGraphNodeProperty *m_prop;

};