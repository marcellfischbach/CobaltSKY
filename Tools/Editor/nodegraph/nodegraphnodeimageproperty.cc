
#include <nodegraph/nodegraphnodeimageproperty.hh>
#include <QPainter>


NodeGraphNodeImageProperty::NodeGraphNodeImageProperty(NodeGraphNode *node)
  : NodeGraphNodeProperty(node)
{

}

NodeGraphNodeImageProperty::~NodeGraphNodeImageProperty()
{

}

QRectF NodeGraphNodeImageProperty::GetMinSize()
{
  if (m_image.isNull())
  {
    return QRectF(0.0f, 0.0f, 0.0f, 0.0f);
  }

  QSize size = m_image.size();
  return QRectF(0.0f, 0.0f, (float)(size.width()+2), (float)(size.height()+ 2));
}

void NodeGraphNodeImageProperty::Paint(QPainter *painter)
{
  if (m_image.isNull())
  {
    return;
  }
  int x = m_bounds.x();
  int y = m_bounds.y();
  bool antiAlias = painter->testRenderHint(QPainter::Antialiasing);
  painter->setRenderHint(QPainter::Antialiasing, false);
  painter->fillRect(x, y, m_image.width() + 1, m_image.height() + 1, QBrush(QColor(0, 0, 0)));
  painter->setPen(QPen(QColor(255, 255, 255), 1.0));
  painter->drawRect(x, y, m_image.width() + 1, m_image.height() + 1);
  painter->drawImage(x+1, y, m_image);
  painter->setRenderHint(QPainter::Antialiasing, antiAlias);
}