#pragma once

#include <QFont>
#include <QString>

class QPainter;

class NodeGraphNodeHeader
{
public:
  NodeGraphNodeHeader();
  ~NodeGraphNodeHeader();

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

  void Paint(QPainter *painter, unsigned width, unsigned height);

private:
  bool m_inShow;
  bool m_inConnected;

  bool m_outShow;
  bool m_outConnected;

  QString m_name;
  QFont m_font;
};