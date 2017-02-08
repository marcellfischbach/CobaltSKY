#pragma once

#include <QFont>
#include <QString>
#include <QRectF>

class QPainter;

class NodeGraphNodeAnchor;
class NodeGraphNodeHeader
{
public:
  NodeGraphNodeHeader();
  ~NodeGraphNodeHeader();

  NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const;
  bool Test(const QPointF &point) const;

  QRectF GetMinSize() const;

  void SetFont(const QFont &font);

  void SetInShow(bool inShow)
  {
    m_inShow = inShow;
  }
  void SetInConnected(bool inConnected)
  {
    m_inConnected = inConnected;
  }
  bool IsInShow() const
  {
    return m_inShow;
  }
  bool IsInConnected() const
  {
    return m_inConnected;
  }


  void SetOutShow(bool outShow)
  {
    m_outShow = outShow;
  }
  void SetOutConnected(bool outConnected)
  {
    m_outConnected = outConnected;
  }
  bool IsOutShow() const
  {
    return m_outShow;
  }
  bool IsOutConnected() const
  {
    return m_outConnected;
  }

  void SetName(const QString &name)
  {
    m_name = name;
  }

  void SetBounds(const QRectF &bounds)
  {
    m_bounds = bounds;
    UpdateBounds();
  }

  void Paint(QPainter *painter);

protected:
  virtual void UpdateBounds();

private:
  bool m_inShow;
  bool m_inConnected;

  bool m_outShow;
  bool m_outConnected;

  QString m_name;
  QFont m_font;

  QRectF m_bounds;

  NodeGraphNodeAnchor *m_leftAnchor;
  NodeGraphNodeAnchor *m_rightAnchor;
};