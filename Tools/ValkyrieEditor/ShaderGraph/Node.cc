
#include <ShaderGraph/Node.hh>
#include <qbrush.h>
#include <qpen.h>
#include <qpainter.h>
#include <qgraphicssceneevent.h>

ShaderGraphNode::ShaderGraphNode()
  : QGraphicsItem()
  , m_fontSize(14)
  , m_inMotion(false)
  , m_node(0)
{

}


bool ShaderGraphNode::Initialize()
{
  if (!m_node)
  {
    return false;
  }


  QFont fnt;
  fnt.setPixelSize(m_fontSize);
  QFontMetrics fm (fnt);

  m_margin = fm.height();
  m_spacing = fm.width("G");


  // calculate the width
  QString nodeName(m_node->GetName().c_str());
  int titleWidth = fm.width(nodeName);

  int maxInput = 0;
  int maxOuput = 0;

  vkSize numInputs = m_node->GetNumberOfInputs();
  vkSize numOutputs = m_node->GetNumberOfOutputs();

  for (vkSize i=0, in=numInputs; i<in; ++i)
  {
    vkSGInput *input = m_node->GetInput(i);
    int width = fm.width(input->GetName().c_str());
    maxInput = maxInput < width ? width : maxInput;
  }

  for (vkSize i=0, in=numOutputs; i<in; ++i)
  {
    vkSGOutput *output= m_node->GetOutput(i);
    int width = fm.width(output->GetName().c_str());
    maxOuput = maxOuput < width ? width : maxOuput;
  }

  int width = titleWidth + 2 * m_margin;
  int inoutWidth = maxInput + maxOuput + m_spacing + 2 * m_margin;

  width = width < inoutWidth ? inoutWidth : width;


  // calculate the height
  int titleHeight = fm.height() * 2;
  int rowHeight = fm.height();


  int numRows = numInputs > numOutputs ? numInputs : numOutputs;

  int height = titleHeight + numRows * rowHeight + rowHeight;

  m_size = QRectF(-width/2.0, -height/2.0, width, height);
  m_inOutStart = m_size.top() + titleHeight;
  m_handleSize = QRectF(m_size.left(), m_size.top(), m_size.width(), fm.height());

  return true;
}


void ShaderGraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  QFont fnt;
  fnt.setPixelSize(m_fontSize);
  QFontMetrics fm (fnt);

  painter->setFont(fnt);

  // draw the background
  QLinearGradient linearGradient(0.0, m_size.top(), 0.0, m_size.top() + fm.height() * 2);
  linearGradient.setColorAt(0.0f, QColor(128, 0, 0));
  linearGradient.setColorAt(1.0f, QColor(0, 0, 0));
  painter->setBrush(QBrush(linearGradient));
  painter->setPen(QPen(QColor(128, 128, 128)));
  painter->drawRoundedRect(m_size, m_fontSize/2, m_fontSize/2);
//  painter->drawRect(m_size);


  // draw the title
  painter->setBrush(QBrush(QColor(0, 0, 0)));
  painter->setPen(QPen(QBrush(QColor(255, 255, 255)), 2.0));
  painter->drawText(m_size.left(), m_size.top(), m_size.width(), fm.height(), Qt::AlignCenter, QString(m_node->GetName().c_str()));

  // draw the inputs
  int startY = m_inOutStart;
  for (vkSize i=0, in=m_node->GetNumberOfInputs(); i<in; ++i)
  {
    vkSGInput *input = m_node->GetInput(i);
    QString str (input->GetName().c_str());
    int width = fm.width(str);
    painter->drawText(m_size.left() + m_margin, startY, width, fm.height(), Qt::AlignHCenter, str);

    if (input->CanInputNode())
    {
      int d = 4;
      painter->drawEllipse(m_size.left() + d, startY + d, fm.height() - 2*d, fm.height() - 2*d);
    }
    startY += fm.height();
  }
  startY = m_inOutStart;
  for (vkSize i=0, in=m_node->GetNumberOfOutputs(); i<in; ++i)
  {
    vkSGOutput *output= m_node->GetOutput(i);
    QString str (output->GetName().c_str());
    int width = fm.width(str);
    painter->drawText(m_size.right() - m_margin - width, startY, width, fm.height(), Qt::AlignHCenter, str);

    int d = 4;
    painter->drawEllipse(m_size.right() - d, startY + d, -(fm.height() - 2*d), fm.height() - 2*d);

    startY += fm.height();
  }
}


void ShaderGraphNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  m_inMotion = m_handleSize.contains(event->pos());
  m_scenePos = event->scenePos();
  QMatrix m = matrix();
  m_pos = QPointF(m.dx(), m.dy());
}

void ShaderGraphNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (m_inMotion)
  {
    QPointF scenePos = event->scenePos();
    QPointF delta = scenePos - m_scenePos;
    QPointF p = m_pos + delta;
    QMatrix m = matrix();
    QMatrix m1 (m.m11(), m.m12(), m.m21(), m.m22(), p.x(), p.y());
    setMatrix(m1);
  }
}

void ShaderGraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  m_inMotion = false;
}


QRectF ShaderGraphNode::boundingRect() const
{
  return m_size;
}


const vkSGNode *ShaderGraphNode::GetNode() const
{
  return m_node;
}

void ShaderGraphNode::SetNode(vkSGNode *node)
{
  VK_SET(m_node, node);
}


