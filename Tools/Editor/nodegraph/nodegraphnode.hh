#pragma once

#include <QPainter>
#include <QRect>
#include <vector>

class NodeGraphNodeHeader;
class NodeGraphNodeProperty;
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