#pragma once

#include <nodegraph/nodegraphnodeproperty.hh>
#include <QColor>


class NodeGraphNodeValueProperty : public NodeGraphNodeProperty
{
public:
  NodeGraphNodeValueProperty();
  virtual ~NodeGraphNodeValueProperty();


  virtual QRectF GetMinSize();
  virtual void Paint(QPainter *painter);
  virtual NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const;

  void SetAnchorShow(bool anchorShow)
  {
    m_anchorShow = anchorShow;
    m_minSizeDirty = true;
  }

  bool IsAnchorShow() const
  {
    return m_anchorShow;
  }

  void SetAnchorConnected(bool anchorConnected)
  {
    m_anchorConnected = anchorConnected;
  }

  bool IsAnchorConnected() const
  {
    return m_anchorConnected;
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

private:
  float m_value;
  bool m_showValue;
  bool m_anchorShow;
  bool m_anchorConnected;

  QColor m_anchorColor;
  QColor m_textColor;

  NodeGraphNodeAnchor *m_anchor;
};