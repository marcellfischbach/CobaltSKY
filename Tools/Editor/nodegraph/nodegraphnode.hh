#pragma once

#include <QPainter>
#include <QRect>
#include <vector>

class NodeGraphNodeAnchor;
class NodeGraphNodeHeader;
class NodeGraphNodeProperty;
class NodeGraphNode
{
public:
  NodeGraphNode();
  ~NodeGraphNode();

  bool TestHandle(const QPointF &point) const;
  NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const;

  void Layout();

  void paint(QPainter *painter);

  void SetLocation(float x, float y);

  void SetBounding(const QRectF &bounding)
  {
    m_bounding = bounding;
  }

  void AddProperty(NodeGraphNodeProperty *nodeProperty);
  void AddInputProperty(NodeGraphNodeProperty *nodeProperty);
  void AddOutputProperty(NodeGraphNodeProperty *nodeProperty);

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
  float m_layoutLeftWidth;
  float m_layoutRightWidth;

  std::vector<NodeGraphNodeProperty *> m_inputProperties;
  std::vector<NodeGraphNodeProperty *> m_outputProperties;

};