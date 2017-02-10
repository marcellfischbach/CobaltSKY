

#include <nodegraph/nodegraphnode.hh>
#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeproperty.hh>

float NodeGraphNode::GridSize = 20.0f;

NodeGraphNode::NodeGraphNode()
  : m_header(new NodeGraphNodeHeader())
  , m_selected(false)
{

}

NodeGraphNode::~NodeGraphNode()
{

}

bool NodeGraphNode::TestHandle(const QPointF &point) const
{
  QPointF locPos = point - m_bounding.topLeft();
  return m_header->Test(locPos);
}

NodeGraphNodeAnchor *NodeGraphNode::GetAnchor(const QPointF &point) const
{
  QPointF locPos = point - m_bounding.topLeft();
  for (NodeGraphNodeProperty *prop : m_inputProperties)
  {
    NodeGraphNodeAnchor *anchor = prop->GetAnchor(locPos);
    if (anchor)
    {
      return anchor;
    }
  }
  for (NodeGraphNodeProperty *prop : m_outputProperties)
  {
    NodeGraphNodeAnchor *anchor = prop->GetAnchor(locPos);
    if (anchor)
    {
      return anchor;
    }
  }
  return 0;
}

void NodeGraphNode::SetLocation(const QPointF &point)
{
  m_position.setX((int)(point.x() / GridSize) * GridSize);
  m_position.setY((int)(point.y() / GridSize) * GridSize);
  UpdateBounding();
}

void NodeGraphNode::SetOffset(const QPointF &point)
{
  m_offset.setX((int)(point.x() / GridSize) * GridSize);
  m_offset.setY((int)(point.y() / GridSize) * GridSize);
  UpdateBounding();
}

void NodeGraphNode::CommitOffset()
{
  m_position += m_offset;
  m_offset = QPointF(0.0f, 0.0f);
  UpdateBounding();
}

void NodeGraphNode::CancelOffset()
{
  m_offset = QPointF(0.0f, 0.0f);
  UpdateBounding();
}

void NodeGraphNode::UpdateBounding()
{
  QPointF p = m_position + m_offset;
  m_bounding = QRectF(p.x(), p.y(), m_bounding.width(), m_bounding.height());
}

void NodeGraphNode::AddProperty(NodeGraphNodeProperty *nodeProperty)
{
  switch (nodeProperty->GetType())
  {
  case eNGNPT_Input:
    AddInputProperty(nodeProperty);
    break;
  case eNGNPT_Output:
    AddOutputProperty(nodeProperty);
    break;
  }
}


void NodeGraphNode::AddInputProperty(NodeGraphNodeProperty *nodeProperty)
{
  nodeProperty->SetType(eNGNPT_Input);
  m_inputProperties.push_back(nodeProperty);
}

void NodeGraphNode::AddOutputProperty(NodeGraphNodeProperty *nodeProperty)
{
  nodeProperty->SetType(eNGNPT_Output);
  m_outputProperties.push_back(nodeProperty);
}

void NodeGraphNode::paint(QPainter *painter)
{
  QTransform currentTransform = painter->transform();

  QTransform localTransform;
  localTransform.translate(m_bounding.left(), m_bounding.top());
  painter->setTransform(localTransform, true);


  painter->fillRect(0, 0, m_bounding.width(), m_bounding.height(), QColor(0, 0, 0, 255));
  if (m_selected)
  {
    QPen pen(QColor(255, 255, 0, 255));
    pen.setWidthF(2.0f);
    painter->setPen(pen);

    painter->drawRect(-1.0f, -1.0f, m_bounding.width() + 2, m_bounding.height() + 2);
  }

  QRectF headerSize = m_header->GetMinSize();
  m_header->Paint(painter);

  int x = 0;
  int y = headerSize.height();
  for (NodeGraphNodeProperty *prop : m_inputProperties)
  {
    QRectF minSize = prop->GetMinSize();
    prop->Paint(painter);
    y += minSize.height();
  }

  y = headerSize.height();
  for (NodeGraphNodeProperty *prop : m_outputProperties)
  {
    QRectF minSize = prop->GetMinSize();
    prop->Paint(painter);
    y += minSize.height();
  }


  painter->setTransform(currentTransform, false);
}

void NodeGraphNode::Layout()
{
  //
  // evaluate the total size of the entire node
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

  //
  // now set the pos and the size of the header and property
  m_header->SetBounds(QRect(0, 0, m_bounding.width(), headerSize.height()));


  int x = 0;
  int y = headerSize.height();
  for (NodeGraphNodeProperty *prop : m_inputProperties)
  {
    QRectF minSize = prop->GetMinSize();
    prop->SetBounds(QRectF(0.0f, (float)y, m_layoutLeftWidth, minSize.height()));
    y += minSize.height();
  }

  y = headerSize.height();
  for (NodeGraphNodeProperty *prop : m_outputProperties)
  {
    QRectF minSize = prop->GetMinSize();
    prop->SetBounds(QRectF(m_bounding.width() - m_layoutRightWidth, y, m_layoutRightWidth, minSize.height()));
    y += minSize.height();
  }


}

void NodeGraphNode::SetAllAnchorsDisconnected()
{
  for (NodeGraphNodeProperty *prop : m_inputProperties)
  {
    prop->SetAllAnchorsDisconnected();
  }
  for (NodeGraphNodeProperty *prop : m_outputProperties)
  {
    prop->SetAllAnchorsDisconnected();
  }
}