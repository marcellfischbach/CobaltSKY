#pragma once

#include <editor/editorexport.hh>
#include <editor/nodegraph/nodegraphnodeproperty.hh>
#include <QColor>


class NodeGraphNodeValueProperty : public NodeGraphNodeProperty
{
public:
  NodeGraphNodeValueProperty(NodeGraphNode *node);
  virtual ~NodeGraphNodeValueProperty();

  virtual void Initialize();

  virtual QRectF GetMinSize();
  virtual void Paint(QPainter *painter);
  virtual NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const;
  NodeGraphNodeAnchor *GetAnchor() const;
  virtual void CollectAllAnchors(QList<NodeGraphNodeAnchor*> &result) const;
  virtual void SetAllAnchorsDisconnected();

  void SetAnchorShow(bool anchorShow)
  {
    m_anchorShow = anchorShow;
    m_minSizeDirty = true;
  }

  bool IsAnchorShow() const
  {
    return m_anchorShow;
  }

  void SetShowValue(bool showValue)
  {
    m_showValue = showValue;
    m_minSizeDirty = true;
  }

  bool IsShowValue() const
  {
    return m_showValue;
  }

  void SetValue(float value)
  {
    m_value = value;
    m_minSizeDirty = true;
  }

  float GetValue() const
  {
    return m_value;
  }

  void SetAnchorColor(const QColor& anchorColor)
  {
    m_anchorColor = anchorColor;
  }

  const QColor &GetAnchorColor() const
  {
    return m_anchorColor;
  }

  void SetTextColor(const QColor& textColor)
  {
    m_textColor = textColor;
  }

  const QColor &GetTextColor() const
  {
    return m_textColor;
  }

protected:
  virtual void UpdateBounds() override;

private:
  void PaintInput(QPainter *painter);
  void PaintOutput(QPainter *painter);
  QString GetZeroStrippedValue();

private:
  float m_value;
  bool m_showValue;
  bool m_anchorShow;

  QColor m_anchorColor;
  QColor m_textColor;

  NodeGraphNodeAnchor *m_anchor;
};