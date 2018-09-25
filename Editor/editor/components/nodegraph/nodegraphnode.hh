#pragma once

#include <editor/editorexport.hh>
#include <QPainter>
#include <QRect>
#include <vector>

class NodeGraphNodeAnchor;
class NodeGraphNodeHeader;
class NodeGraphNodeProperty;
class EDITOR_API NodeGraphNode
{
public:
  NodeGraphNode();
  virtual ~NodeGraphNode();

  bool TestHandle(const QPointF &point) const;
  NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const;
  QList<NodeGraphNodeAnchor*> GetAllAnchors() const;

  void Layout();

  void paint(QPainter *painter);

  void SetLocation(const QPointF &point);
  const QPointF &GetLocation() const
  {
    return m_position;
  }
  void SetOffset(const QPointF &point);
  void CommitOffset();
  void CancelOffset();

  void UpdateBounding();

  void SetBounding(const QRectF &bounding)
  {
    m_bounding = bounding;
  }

  void AddProperty(NodeGraphNodeProperty *nodeProperty);
  void AddInputProperty(NodeGraphNodeProperty *nodeProperty);
  void AddOutputProperty(NodeGraphNodeProperty *nodeProperty);
  NodeGraphNodeProperty *GetInputProperty(int idx);
  NodeGraphNodeProperty *GetOutputProperty(int idx);

  const QRectF &GetBounding() const
  {
    return m_bounding;
  }

  NodeGraphNodeHeader* GetHeader()
  {
    return m_header;
  }

  void SetSelected(bool selected)
  {
    m_selected = selected;
  }

  bool IsSelected() const
  {
    return m_selected;
  }

  void SetAllAnchorsDisconnected();

private:
  bool m_selected;
  QPointF m_position;
  QPointF m_offset;
  QRectF m_bounding;
  NodeGraphNodeHeader *m_header;
  float m_layoutLeftWidth;
  float m_layoutRightWidth;

  std::vector<NodeGraphNodeProperty *> m_inputProperties;
  std::vector<NodeGraphNodeProperty *> m_outputProperties;

  static float GridSize;

};