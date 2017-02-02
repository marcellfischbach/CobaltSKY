

#include <nodegraph/nodegraphnodeheader.hh>
#include <QFontMetrics>
#include <QPainter>

#define CONNECTOR_HEIGHT 20
#define CONNECTOR_WIDTH 20
#define CONNECTOR_SPACING 4

NodeGraphNodeHeader::NodeGraphNodeHeader()
  : m_inShow(false)
  , m_inConnected(false)
  , m_outShow(false)
  , m_outConnected(false)
  , m_name("Unnamed")
{

}


void NodeGraphNodeHeader::SetFont(const QFont &font)
{
  m_font = font;
}

QRectF NodeGraphNodeHeader::GetMinSize() const
{
  unsigned height = 0;
  unsigned width = 0;
  if (m_inShow)
  {
    height = height < CONNECTOR_HEIGHT ? CONNECTOR_HEIGHT : height;
    width += CONNECTOR_WIDTH;
  }

  QFontMetrics fm(m_font);
  unsigned textHeight = fm.height();
  height = height < textHeight ? textHeight : height;
  width += fm.width(m_name);
  width += CONNECTOR_SPACING * 2;
  if (m_outShow)
  {
    height = height < CONNECTOR_HEIGHT ? CONNECTOR_HEIGHT : height;
    width += CONNECTOR_WIDTH;
  }

  return QRectF(0, 0, width, height);
}


void NodeGraphNodeHeader::Paint(QPainter *painter, unsigned width, unsigned height)
{
  painter->setPen(QPen(QColor(64, 64, 64)));
  unsigned x = 0;
  if (m_inShow)
  {
    unsigned px = 0;
    float ch = (float)height / 2.0f;
    QPainterPath path;
    path.moveTo(px + 4, ch - 6);
    path.lineTo(px + 10, ch - 6);
    path.lineTo(px + 16, ch);
    path.lineTo(px + 10, ch + 6);
    path.lineTo(px + 4, ch + 6);
    path.lineTo(px + 4, ch - 6);
    painter->drawPath(path);
    x += CONNECTOR_WIDTH;
  }

  x += CONNECTOR_SPACING;
  painter->drawText(x, 0, width, height, Qt::AlignLeft | Qt::AlignVCenter, m_name);

  if (m_outShow)
  {
    unsigned px = width - CONNECTOR_WIDTH;
    float ch = (float)height / 2.0f;
    QPainterPath path;
    path.moveTo(px + 4, ch - 6);
    path.lineTo(px + 10, ch - 6);
    path.lineTo(px + 16, ch);
    path.lineTo(px + 10, ch + 6);
    path.lineTo(px + 4, ch + 6);
    path.lineTo(px + 4, ch - 6);
    painter->drawPath(path);
    x += CONNECTOR_WIDTH + CONNECTOR_SPACING;
  }
}