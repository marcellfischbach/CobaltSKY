

#include <nodegraph/nodegraphnodevalueproperty.hh>
#include <nodegraph/nodegraphnodeanchor.hh>
#include <QPainter>

#define ANCHOR_RADIUS 20
#define ANCHOR_RADIUS_2 10
#define ANCHOR_SPACING 4


NodeGraphNodeValueProperty::NodeGraphNodeValueProperty(NodeGraphNode *node)
  : NodeGraphNodeProperty(node)
  , m_anchorColor(255, 255, 255, 255)
  , m_textColor(255, 255, 255, 255)
  , m_anchor(0)
  , m_anchorShow(false)
{

}

NodeGraphNodeValueProperty::~NodeGraphNodeValueProperty()
{

}

void NodeGraphNodeValueProperty::Initialize()
{
  if (!m_anchorShow && m_anchor)
  {
    delete m_anchor;
    m_anchor = 0;
  }
  else if (m_anchorShow && !m_anchor)
  {
    m_anchor = new NodeGraphNodeAnchor(m_node);
  }

  if (m_anchor)
  {
    m_anchor->SetType(m_type);
  }
}

void NodeGraphNodeValueProperty::UpdateBounds()
{
  if (!m_anchor)
  {
    return;
  }

  int x = (int)m_bounds.x();
  int y = (int)m_bounds.y();
  unsigned width = (unsigned)m_bounds.width();
  unsigned height = (unsigned)m_bounds.height();

  switch (m_type)
  {
  case eNGNPT_Input:
    m_anchor->SetBounds(QRectF(x, y + height/2 - ANCHOR_RADIUS_2, ANCHOR_RADIUS, ANCHOR_RADIUS));
    break;
  case eNGNPT_Output:
    m_anchor->SetBounds(QRectF(x + width - ANCHOR_RADIUS, y + height / 2 - ANCHOR_RADIUS_2, ANCHOR_RADIUS, ANCHOR_RADIUS));
    break;
  }
}

QRectF NodeGraphNodeValueProperty::GetMinSize()
{
  if (m_minSizeDirty)
  {
    unsigned width = 0;
    unsigned height = 0;
    if (m_anchorShow)
    {
      width += ANCHOR_RADIUS;
      height = height < ANCHOR_RADIUS ? ANCHOR_RADIUS : height;
    }

    QString text = " ";
    if (m_showValue && m_type == eNGNPT_Input && (!m_anchorShow || !m_anchor || !m_anchor->IsConnected()))
    {
      text += QString("[%1] ").arg(GetZeroStrippedValue());
    }
    text += m_name;

    QFontMetrics fm(m_font);
    unsigned textHeight = fm.height();
    height = height < textHeight ? textHeight : height;
    width += fm.width(text);
    width += ANCHOR_SPACING;

    m_minSize = QRectF(0, 0, width, height);
    m_minSizeDirty = false;
  }
  return m_minSize;
}


void NodeGraphNodeValueProperty::Paint(QPainter *painter)
{
  switch (m_type)
  {
  case eNGNPT_Input:
    PaintInput(painter);
    break;
  case eNGNPT_Output:
    PaintOutput(painter);
    break;
  }
}


void NodeGraphNodeValueProperty::PaintInput(QPainter *painter)
{
  int x = (int)m_bounds.x();
  int y = (int)m_bounds.y();
  unsigned width = (unsigned)m_bounds.width();
  unsigned height = (unsigned)m_bounds.height();
  int lx = x;
  if (m_anchor)
  {
    painter->setPen(QPen(m_anchorColor));
    painter->drawEllipse(QPointF(x + ANCHOR_RADIUS_2 + 0.5f, y + height / 2 + 0.5f), ANCHOR_RADIUS_2 - 4, ANCHOR_RADIUS_2 - 4);
    lx += ANCHOR_RADIUS;

    if (m_anchor->IsConnected())
    {
      QPainterPath path;
      path.addEllipse(QPointF(x + ANCHOR_RADIUS_2 + 0.5f, y + height / 2 + 0.5f), ANCHOR_RADIUS_2 - 6, ANCHOR_RADIUS_2 - 6);
      painter->fillPath(path, QBrush(m_anchorColor));
    }
  }

  QString text = " ";
  if (m_showValue && m_type == eNGNPT_Input && (!m_anchorShow || !m_anchor->IsConnected()))
  {
    text += QString("[%1] ").arg(GetZeroStrippedValue());
  }
  text += m_name;
  painter->setPen(QPen(m_textColor));
  painter->drawText(lx, y, x + width - lx, height, Qt::AlignLeft | Qt::AlignVCenter, text);
}


void NodeGraphNodeValueProperty::PaintOutput(QPainter *painter)
{
  int x = (int)m_bounds.x();
  int y = (int)m_bounds.y();
  unsigned width = (unsigned)m_bounds.width();
  unsigned height = (unsigned)m_bounds.height();

  painter->setPen(QPen(m_anchorColor));
  painter->drawEllipse(QPoint(x + width - ANCHOR_RADIUS_2 + 0.5f, y + height / 2 + 0.5f), ANCHOR_RADIUS_2 - 4, ANCHOR_RADIUS_2 - 4);
  if (m_anchor && m_anchor->IsConnected())
  {
    QPainterPath path;
    path.addEllipse(QPointF(x + width - ANCHOR_RADIUS_2 , y + height / 2 ), ANCHOR_RADIUS_2 - 6, ANCHOR_RADIUS_2 - 6);
    painter->fillPath(path, QBrush(m_anchorColor));
  }
  int rx = x + width - ANCHOR_RADIUS;

  painter->setPen(QPen(m_textColor));
  painter->drawText(x + ANCHOR_SPACING, y, rx - (x + ANCHOR_SPACING), height, Qt::AlignLeft | Qt::AlignVCenter, m_name);

}

NodeGraphNodeAnchor *NodeGraphNodeValueProperty::GetAnchor(const QPointF &point) const
{
  if (m_anchor && m_anchor->Test(point))
  {
    return m_anchor;
  }
  return 0;
}


NodeGraphNodeAnchor *NodeGraphNodeValueProperty::GetAnchor() const
{
  return m_anchor;
}

void NodeGraphNodeValueProperty::CollectAllAnchors(QList<NodeGraphNodeAnchor*> &result) const
{
  result.append(m_anchor);
}

void NodeGraphNodeValueProperty::SetAllAnchorsDisconnected()
{
  if (m_anchor)
  {
    m_anchor->SetConnected(false);
  }
}


QString NodeGraphNodeValueProperty::GetZeroStrippedValue()
{
  QString str("%1");
  str = str.arg(m_value, 0, 'f');
  while (str.length() > 2 && str[str.length() - 1] == '0' && str[str.length() - 2] != '.') {
    str = str.left(str.length() - 1);
  }
  return str;
}