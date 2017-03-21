

#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeanchor.hh>
#include <QFontMetrics>
#include <QPainter>

#define CONNECTOR_HEIGHT 20
#define CONNECTOR_WIDTH 20
#define CONNECTOR_SPACING 4
#define HEADER_TITLE_SPACING  2

NodeGraphNodeHeader::NodeGraphNodeHeader()
  : m_inShow(false)
  , m_inConnected(false)
  , m_outShow(false)
  , m_outConnected(false)
  , m_name("Unnamed")
  , m_leftAnchor(0)
  , m_rightAnchor(0)
  , m_color0(0, 0, 0)
  , m_color1(0, 0, 0)
  , m_textColor(255, 255, 255)
{
  m_font.setPointSize(10);
  m_subFont.setPointSize(8);
  m_subFont.setItalic(true);
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

void NodeGraphNodeHeader::SetSubFont(const QFont &subFont)
{
  m_subFont = subFont;
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
  QFontMetrics fmSub(m_subFont);
  unsigned textHeight = fm.height();
  unsigned textWidth = fm.width(m_name);
  textHeight += HEADER_TITLE_SPACING;
  if (!m_subName.isEmpty())
  {
    textHeight += fmSub.height();
    textHeight += HEADER_TITLE_SPACING;
    unsigned subWidth = fmSub.width(m_subName);
    textWidth = textWidth < subWidth ? subWidth : textWidth;
  }
  textHeight += HEADER_TITLE_SPACING;


  height = height < textHeight ? textHeight : height;
  width += textWidth;
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
  linGrad.setColorAt(0.0f, m_color0);
  linGrad.setColorAt(1.0f, m_color1);
  painter->fillRect(x, y, width, height, linGrad);
  painter->setPen(QPen(m_textColor));
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

  painter->setFont(m_font);
  painter->drawText(lx, y + HEADER_TITLE_SPACING, rx - lx, height- 2* HEADER_TITLE_SPACING, Qt::AlignLeft | Qt::AlignTop, m_name);
  painter->setFont(m_subFont);
  painter->drawText(lx, y + HEADER_TITLE_SPACING, rx - lx, height - 2 * HEADER_TITLE_SPACING, Qt::AlignLeft | Qt::AlignBottom, m_subName);
  painter->setFont(QFont());
}

