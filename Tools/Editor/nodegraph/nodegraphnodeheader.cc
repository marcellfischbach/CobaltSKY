

#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeanchor.hh>
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
  , m_leftAnchor(0)
  , m_rightAnchor(0)
{

}

NodeGraphNodeHeader::~NodeGraphNodeHeader()
{

}

void NodeGraphNodeHeader::UpdateBounds()
{
}

NodeGraphNodeAnchor *NodeGraphNodeHeader::GetAnchor(const QPointF &anchor) const
{
  return 0;
}

bool NodeGraphNodeHeader::Test(const QPointF &point) const
{
  if (m_leftAnchor && m_leftAnchor->Test(point))
  {
    return false;
  }
  if (m_rightAnchor && m_rightAnchor->Test(point))
  {
    return false;
  }
  return m_bounds.contains(point);
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


void NodeGraphNodeHeader::Paint(QPainter *painter)
{
  int x = (int)m_bounds.x();
  int y = (int)m_bounds.y();
  unsigned width = (unsigned)m_bounds.width();
  unsigned height = (unsigned)m_bounds.height();

  QLinearGradient linGrad(0, 0, width, 0);
  linGrad.setColorAt(0.0f, QColor(255, 0, 0));
  linGrad.setColorAt(1.0f, QColor(0, 0, 0));
  painter->fillRect(x, y, width, height, linGrad);
  painter->setPen(QPen(QColor(255, 255, 255)));
  unsigned lx = x + CONNECTOR_SPACING;
  unsigned rx = x + width - CONNECTOR_SPACING;

  if (m_inShow)
  {
    unsigned px = x;
    float ch = (float)height / 2.0f;
    QPainterPath path;
    path.moveTo(px + 4, ch - 6);
    path.lineTo(px + 10, ch - 6);
    path.lineTo(px + 16, ch);
    path.lineTo(px + 10, ch + 6);
    path.lineTo(px + 4, ch + 6);
    path.lineTo(px + 4, ch - 6);
    painter->drawPath(path);
    lx += CONNECTOR_WIDTH;
  }

  if (m_outShow)
  {
    unsigned px = x + width - CONNECTOR_WIDTH;
    float ch = (float)height / 2.0f;
    QPainterPath path;
    path.moveTo(px + 4, ch - 6);
    path.lineTo(px + 10, ch - 6);
    path.lineTo(px + 16, ch);
    path.lineTo(px + 10, ch + 6);
    path.lineTo(px + 4, ch + 6);
    path.lineTo(px + 4, ch - 6);
    painter->drawPath(path);
    rx -= CONNECTOR_WIDTH;
  }


  painter->drawText(lx, y, rx - lx, height, Qt::AlignLeft | Qt::AlignVCenter, m_name);

}

