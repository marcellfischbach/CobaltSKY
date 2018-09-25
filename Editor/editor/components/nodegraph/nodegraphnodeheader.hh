#pragma once

#include <editor/editorexport.hh>
#include <QColor>
#include <QFont>
#include <QString>
#include <QRectF>

class QPainter;

class NodeGraphNodeAnchor;
class EDITOR_API NodeGraphNodeHeader
{
public:
  NodeGraphNodeHeader();
  ~NodeGraphNodeHeader();

  NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const;
  bool Test(const QPointF &point) const;

  QRectF GetMinSize() const;

  void SetFont(const QFont &font);
  void SetSubFont(const QFont &subFont);

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

  void SetSubName(const QString &subName)
  {
    m_subName = subName;
  }


  void SetBounds(const QRectF &bounds)
  {
    m_bounds = bounds;
    UpdateBounds();
  }

  void SetColor0(const QColor &color)
  {
    m_color0 = color;
  }
  void SetColor1(const QColor &color)
  {
    m_color1 = color;
  }
  void SetTestColor(const QColor &color)
  {
    m_textColor = color;
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
  QString m_subName;
  QFont m_font;
  QFont m_subFont;

  QRectF m_bounds;

  NodeGraphNodeAnchor *m_leftAnchor;
  NodeGraphNodeAnchor *m_rightAnchor;

  QColor m_color0;
  QColor m_color1;
  QColor m_textColor;
};