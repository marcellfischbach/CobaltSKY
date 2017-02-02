#pragma once

#include <nodegraph/nodegraphnodepropertytype.hh>

#include <QFont>
#include <QRectF>
#include <QString>

class QPainter;
class NodeGraphNodeProperty
{
public:
  NodeGraphNodeProperty();
  ~NodeGraphNodeProperty();

  QRectF GetMinSize() const;

  void Paint(QPainter *painter);

  void SetFont(const QFont &font)
  {
    m_font = font;
  }

  void SetType(NodeGraphNodePropertyType type)
  {
    m_type = type;
  }
  NodeGraphNodePropertyType GetType() const
  {
    return m_type;
  }

  void SetName(const QString &name)
  {
    m_name = name;
  }
  const QString &GetName() const
  {
    return m_name;
  }

  void SetAnchorShow(bool anchorShow)
  {
    m_anchorShow = anchorShow;
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
  }

  bool IsShowValue() const
  {
    return m_showValue;
  }

  void SetValue(float value)
  {
    m_value = value;
  }

  float GetValue() const
  {
    return m_value;
  }
private:
  NodeGraphNodePropertyType m_type;
  QString m_name;
  QFont m_font;
  float m_value;
  bool m_showValue;
  bool m_anchorShow;
  bool m_anchorConnected;
};