

#include <nodegraph/nodegraphnode.hh>
#include <nodegraph/nodegraphnodeheader.hh>

NodeGraphNode::NodeGraphNode()
  : m_header(new NodeGraphNodeHeader())
{

}

NodeGraphNode::~NodeGraphNode()
{

}

void NodeGraphNode::SetLocation(float x, float y)
{
  m_bounding = QRectF(x, y, m_bounding.width(), m_bounding.height());
}

void NodeGraphNode::paint(QPainter *painter)
{
  QTransform currentTransform = painter->transform();

  QTransform localTransform;
  localTransform.translate(m_bounding.left(), m_bounding.top());
  painter->setTransform(localTransform, true);


  painter->fillRect(0, 0, m_bounding.width(), m_bounding.height(), QColor(128, 128, 128, 255));

  m_header->Paint(painter, m_bounding.width(), m_bounding.height());

  painter->setTransform(currentTransform, false);
}

void NodeGraphNode::Layout()
{
  QRectF headerSize = m_header->GetMinSize();
  m_bounding.setWidth(headerSize.width());
  m_bounding.setHeight(headerSize.height());
}