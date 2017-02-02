

#include <nodegraph/nodegraphnodeproperty.hh>
#include <QPainter>

#define ANCHOR_WIDTH 20
#define ANCHOR_HEIGHT 20
#define ANCHOR_SPACING 4

NodeGraphNodeProperty::NodeGraphNodeProperty()
{

}

NodeGraphNodeProperty::~NodeGraphNodeProperty()
{

}

QRectF NodeGraphNodeProperty::GetMinSize() const
{
  unsigned width = 0;
  unsigned height = 0;
  if (m_anchorShow)
  {
    width += ANCHOR_WIDTH;
    height = height < ANCHOR_HEIGHT ? ANCHOR_HEIGHT : height;
  }

  QString text = "";
  if (m_showValue && m_type == eNGNPT_Input)
  {
    text += QString("[%1] ").arg(m_value, 0, 'f');
  }
  text += m_value;

  QFontMetrics fm(m_font);
  unsigned textHeight = fm.height();
  height = height < textHeight ? textHeight : height;
  width += fm.width(text);
  width += ANCHOR_SPACING * 2;

  return QRectF(0, 0, width, height);
}