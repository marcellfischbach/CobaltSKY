
#include <ShaderGraph/Node.hh>
#include <qbrush.h>
#include <qpen.h>
#include <qpainter.h>

ShaderGraphNode::ShaderGraphNode()
  : QGraphicsItem()
{

}


bool ShaderGraphNode::Initialize()
{
  if (!m_node)
  {
    return false;
  }

  /*
  QGraphicsRectItem *rectItem = new QGraphicsRectItem(this);
  rectItem->setRect(-100.0f, -50.0f, 100.0f, 50.0f);
  rectItem->setBrush(QBrush(QColor(128, 128, 128)));
  rectItem->setPen(QPen(QColor(0, 0, 0)));
  rectItem->setTransform(QTransform(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));

  QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(this);
  text->setText(QString(m_node->GetName().c_str()));
  text->setTransform(QTransform(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));

  addToGroup(rectItem);
  addToGroup(text);
  */
  return true;
}


void ShaderGraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->setBrush(QBrush(QColor(128, 128, 128)));
  painter->setPen(QPen(QColor(0, 0, 0)));
  painter->drawRect(-200, -100, 400, 200);

  painter->setBrush(QBrush(QColor(255, 255, 255)));
  painter->setPen(QPen(QColor(255, 255, 255)));
  painter->drawText(-200, -100, 400, 200, Qt::AlignTop | Qt::AlignHCenter, QString(m_node->GetName().c_str()));
}

QRectF ShaderGraphNode::boundingRect() const
{
  return QRectF(-200.0f, -100, 200, 100);
}


const vkSGNode *ShaderGraphNode::GetNode() const
{
  return m_node;
}

void ShaderGraphNode::SetNode(vkSGNode *node)
{
  m_node = node;
}


