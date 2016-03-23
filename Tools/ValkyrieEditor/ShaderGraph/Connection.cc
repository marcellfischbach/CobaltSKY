
#include <ShaderGraph/Connection.hh>
#include <ShaderGraph/Node.hh>
#include <qpen.h>
#include <qbrush.h>

ShaderGraphConnection::ShaderGraphConnection(ShaderGraphNode *out, int outIndex, ShaderGraphNode *in, int inIndex)
  : QObject()
  , m_out(out)
  , m_outIndex(outIndex)
  , m_in(in)
  , m_inIndex(inIndex)
{
  m_pathItem = new QGraphicsPathItem ();
  m_pathItem->setPen(QPen(QBrush(QColor(255, 255, 255)), 2.0f));

  UpdateValues ();
}


void ShaderGraphConnection::UpdateValues()
{
  QPointF pointA = m_out->GetOutputAnchor(m_outIndex);
  QPointF pointB = m_in->GetInputAnchor(m_inIndex);
  float cx = (pointA.x() + pointB.x()) / 2.0f;

  QPainterPath path;
  path.moveTo(pointA);
  path.cubicTo(QPointF(cx, pointA.y()), QPointF(cx, pointB.y()), pointB);
  m_pathItem->setPath(path);
}

QGraphicsPathItem *ShaderGraphConnection::GetItem()
{
  return m_pathItem;
}
