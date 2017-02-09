#pragma once

#include <nodegraph/nodegraphnodepropertytype.hh>

#include <QFont>
#include <QRectF>
#include <QString>

class QPainter;
class NodeGraphNode;
class NodeGraphNodeAnchor;
class NodeGraphNodeProperty
{
public:
  NodeGraphNodeProperty(NodeGraphNode *node);
  ~NodeGraphNodeProperty();

  virtual QRectF GetMinSize() = 0;
  virtual void Paint(QPainter *painter) = 0;
  virtual NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const = 0;

  virtual void SetAllAnchorsDisconnected() = 0;

  void SetFont(const QFont &font)
  {
    m_font = font;
    m_minSizeDirty = true;
  }

  void SetType(NodeGraphNodePropertyType type)
  {
    m_type = type;
    m_minSizeDirty = true;
  }
  NodeGraphNodePropertyType GetType() const
  {
    return m_type;
  }

  void SetName(const QString &name)
  {
    m_name = name;
    m_minSizeDirty = true;
  }
  const QString &GetName() const
  {
    return m_name;
  }


  void SetBounds(const QRectF &bounds)
  {
    m_bounds = bounds;
    UpdateBounds();
  }

  NodeGraphNode *GetNode() const
  {
    return m_node;
  }

protected:
  virtual void UpdateBounds() { }

protected:
  NodeGraphNode *m_node;
  NodeGraphNodePropertyType m_type;
  QString m_name;
  QFont m_font;
  bool m_minSizeDirty;
  QRectF m_minSize;

  QRectF m_bounds;
};