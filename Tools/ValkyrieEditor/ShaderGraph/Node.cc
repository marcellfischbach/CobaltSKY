
#include <ShaderGraph/Node.hh>
#include <qbrush.h>
#include <qpen.h>
#include <qpainter.h>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>
#include <qtextdocument.h>

ShaderGraphNode2 *ShaderGraphNode2::selectedNode = 0;


ShaderGraphNode::ShaderGraphNode(vkSGNode *node)
  : QObject()
  , m_node(node)
  , m_titleFontSize(12)
  , m_inoutFontSize(11)
{

  QFont fntTitle;
  fntTitle.setPixelSize(m_titleFontSize);
  QFontMetrics fmTitle (fntTitle);

  // the interface font (interface => inputs and outputs)
  QFont fntIF;
  fntIF.setPixelSize(m_inoutFontSize);
  QFontMetrics fmIF(fntIF);

  QString nodeTitle(node->GetName().c_str());

  int margin = 5;
  int spacing = 3;
  int constInputWidth = fmIF.width("GGGG");
  int constInputHeight = fmIF.height() + fmIF.descent();
  int elipseSize = fmIF.height() ;
  int titleWidth = fmTitle.width(nodeTitle) + 2*margin;


  m_parent = new QGraphicsItemGroup();
  m_parent->setHandlesChildEvents(false);


  m_background = new QGraphicsRectItem();
  m_background->setPen(QPen(QColor(255, 255, 255)));
  m_background->setBrush(QBrush(QColor(64, 64, 64)));
  m_parent->addToGroup(m_background);

  m_title = new QGraphicsSimpleTextItem (m_background);
  m_title->setFont(fntTitle);
  m_title->setText(nodeTitle);
  m_title->setPen(QPen(Qt::NoPen));
  m_title->setBrush(QBrush(QColor(255, 255, 255)));
  m_title->setPos(margin, margin);
  m_parent->addToGroup(m_title);

  int titleHeight = fmTitle.height() + 2*spacing + margin ;
  int posY = titleHeight;


  int inputWidth = 0;
  int inputHeight = 0;
  for (size_t i=0, in=m_node->GetNumberOfInputs(); i<in; ++i)
  {
    vkSGInput *input = m_node->GetInput(i);
    QString inputName (input->GetName().c_str());

    int thisWidth = margin + elipseSize + spacing;

    Anchor anchor;
    memset(&anchor, 0, sizeof(Anchor));
    if (input->CanInputNode())
    {
      anchor.anchor = new QGraphicsEllipseItem(m_background);
      anchor.anchor->setRect(margin, posY, elipseSize, elipseSize);
      anchor.anchor->setPen(QPen(QBrush(QColor(255, 255, 255)), 2.0f));
      anchor.anchor->setBrush(QBrush(Qt::NoBrush));
      m_parent->addToGroup(anchor.anchor);
    }
    anchor.text = new QGraphicsSimpleTextItem(m_background);
    anchor.text->setFont(fntIF);
    anchor.text->setText(inputName);
    anchor.text->setPos(thisWidth, posY);
    anchor.text->setPen(QPen(Qt::NoPen));
    anchor.text->setBrush(QBrush(QColor(255, 255, 255)));
    m_parent->addToGroup(anchor.text);

    thisWidth += fmIF.width(inputName);
    if (input->CanInputConst())
    {
      thisWidth += spacing;
      anchor.editValueBackground = new QGraphicsRectItem(m_background);
      anchor.editValueBackground->setPen(QPen(QColor(0, 0, 0)));
      anchor.editValueBackground->setBrush(QBrush(QColor(255, 255, 255)));
      anchor.editValueBackground->setRect(thisWidth, posY, constInputWidth, constInputHeight);

      anchor.editValue = new QGraphicsTextItem(anchor.editValueBackground);
      anchor.editValue->setPos(thisWidth, posY);
      anchor.editValue->setPlainText("1");
      anchor.editValue->setTextInteractionFlags(Qt::TextEditorInteraction);
      anchor.editValue->setTextWidth(constInputWidth);
      anchor.editValue->setFont(fntIF);
      printf ("Size: %d\n", anchor.editValue->document()->documentMargin());
      fflush(stdout);

      m_parent->addToGroup(anchor.editValueBackground);
      m_parent->addToGroup(anchor.editValue);

      thisWidth += constInputWidth;
      posY += fmIF.descent();
    }

    // add a margin on the far side
    thisWidth += margin;

    inputWidth = inputWidth < thisWidth ? thisWidth : inputWidth;

    posY += fmIF.height() + spacing;
    inputHeight += fmIF.height() + spacing;
  }

  int width = inputWidth > titleWidth ? inputWidth : titleWidth;
  int height = posY;



  m_background->setRect(0, 0, width, height);


}

QGraphicsItem *ShaderGraphNode::GetItem()
{
  return m_parent;
}


ShaderGraphNode2::ShaderGraphNode2()
  : QGraphicsItem()
  , m_fontSize(14)
  , m_inMotion(false)
  , m_node(0)
{

}


bool ShaderGraphNode2::Initialize()
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


  int startY = m_inOutStart;
  for (vkSize i=0, in=m_node->GetNumberOfInputs(); i<in; ++i)
  {
    vkSGInput *input = m_node->GetInput(i);
    if (input->CanInputNode())
    {
      int d = 4;\
      m_inputRects.append(QRectF(m_size.left() + d, startY + d, fm.height() - 2*d, fm.height() - 2*d));
      //painter->drawEllipse(m_size.left() + d, startY + d, fm.height() - 2*d, fm.height() - 2*d);
    }
    startY += fm.height();
  }
  startY = m_inOutStart;
  for (vkSize i=0, in=m_node->GetNumberOfOutputs(); i<in; ++i)
  {

    int d = 4;
    m_outputRects.append(QRectF(m_size.right() - d -(fm.height() - 2*d), startY + d, (fm.height() - 2*d), fm.height() - 2*d));

    startY += fm.height();
  }

  return true;
}


void ShaderGraphNode2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  QFont fnt;
  fnt.setPixelSize(m_fontSize);
  QFontMetrics fm (fnt);

  painter->setFont(fnt);

  // draw the background
  QLinearGradient linearGradient(0.0, m_size.top(), 0.0, m_size.top() + fm.height() * 2);
  if (this == ShaderGraphNode2::GetSelectedNode())
  {
    linearGradient.setColorAt(0.0f, QColor(150, 0, 0));
    linearGradient.setColorAt(1.0f, QColor(16, 16, 16));
    painter->setPen(QPen(QBrush(QColor(196, 196, 196)), 2.0));
  }
  else
  {
    linearGradient.setColorAt(0.0f, QColor(128, 0, 0));
    linearGradient.setColorAt(1.0f, QColor(0, 0, 0));
    painter->setPen(QPen(QBrush(QColor(64, 64, 64)), 1.0));
  }
  painter->setBrush(QBrush(linearGradient));
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
    startY += fm.height();
  }
  startY = m_inOutStart;
  for (vkSize i=0, in=m_node->GetNumberOfOutputs(); i<in; ++i)
  {
    vkSGOutput *output= m_node->GetOutput(i);
    QString str (output->GetName().c_str());
    int width = fm.width(str);
    painter->drawText(m_size.right() - m_margin - width, startY, width, fm.height(), Qt::AlignHCenter, str);
    startY += fm.height();
  }

  foreach (QRectF rect, m_inputRects)
  {
    painter->drawEllipse(rect);
  }
  foreach (QRectF rect, m_outputRects)
  {
    painter->drawEllipse(rect);
  }
}


void ShaderGraphNode2::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  m_inMotion = m_handleSize.contains(event->pos());
  m_scenePos = event->scenePos();
  QMatrix m = matrix();
  m_pos = QPointF(m.dx(), m.dy());

  bool hit = false;
  for (size_t i=0; i<m_inputRects.size() && !hit; ++i)
  {
    if (m_inputRects[i].contains(event->pos()))
    {
      hit = true;
      QPointF p = event->pos();
      p = mapToScene(p);
      m_connecting = true;
//      emit startConnectionInput(p, i);
    }
  }
  for (size_t i=0; i<m_outputRects.size() && !hit; ++i)
  {
    if (m_outputRects[i].contains(event->pos()))
    {
      hit = true;
      QPointF p = event->pos();
      p = mapToScene(p);
      m_connecting = true;
//      emit startConnectionOutput(p, i);
    }
  }


}

void ShaderGraphNode2::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (m_inMotion)
  {
    ShaderGraphNode2::SetSelectedNode(this);
    QPointF scenePos = event->scenePos();
    QPointF delta = scenePos - m_scenePos;
    QPointF p = m_pos + delta;
    QMatrix m = matrix();
    QMatrix m1 (m.m11(), m.m12(), m.m21(), m.m22(), p.x(), p.y());
    setMatrix(m1);
  }
  if (m_connecting)
  {
    QPointF p = event->pos();
    p = mapToScene(p);
//    emit processConnection(p);
  }
}

void ShaderGraphNode2::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (m_inMotion)
  {
    ShaderGraphNode2::SetSelectedNode(this);
  }
  m_inMotion = false;
  m_connecting = false;

}


QRectF ShaderGraphNode2::boundingRect() const
{
  return m_size;
}


const vkSGNode *ShaderGraphNode2::GetNode() const
{
  return m_node;
}

void ShaderGraphNode2::SetNode(vkSGNode *node)
{
  VK_SET(m_node, node);
}




void ShaderGraphNode2::SetSelectedNode(ShaderGraphNode2 *selectedNode)
{
  if (ShaderGraphNode2::selectedNode)
  {
    ShaderGraphNode2::selectedNode->update();
  }
  ShaderGraphNode2::selectedNode = selectedNode;
  if (ShaderGraphNode2::selectedNode)
  {
    ShaderGraphNode2::selectedNode->update();
  }
}

ShaderGraphNode2 *ShaderGraphNode2::GetSelectedNode()
{
  return ShaderGraphNode2::selectedNode;
}
