

#include <nodegraph/nodegraphnode.hh>
#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeproperty.hh>

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

void NodeGraphNode::AddInputProperty(NodeGraphNodeProperty *nodeProperty)
{
  m_inputProperties.push_back(nodeProperty);
}

void NodeGraphNode::AddOutputProperty(NodeGraphNodeProperty *nodeProperty)
{
  m_outputProperties.push_back(nodeProperty);
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

  float leftWidth = 0.0f;
  float leftHeight = 0.0f;
  float rightWidth = 0.0f;
  float rightHeight = 0.0f;

  for (NodeGraphNodeProperty *prop : m_inputProperties)
  {
    QRectF size = prop->GetMinSize();
    leftWidth = size.width() > leftWidth ? size.width() : leftWidth;
    leftHeight += size.height();
  }

  for (NodeGraphNodeProperty *prop : m_outputProperties)
  {
    QRectF size = prop->GetMinSize();
    rightWidth = size.width() > rightWidth ? size.width() : rightWidth;
    rightHeight += size.height();
  }

  float propsHeight = leftHeight > rightHeight ? leftHeight : rightHeight;
  float propsWidth = leftWidth + rightWidth;

  float boundingHeight = headerSize.height() + propsHeight;
  float boundingWidth = headerSize.width() > propsWidth ? headerSize.width() : propsWidth;

  m_layoutLeftWidth = leftWidth;
  m_layoutRightWidth = rightWidth;

  m_bounding.setWidth(boundingWidth);
  m_bounding.setHeight(boundingHeight);

}