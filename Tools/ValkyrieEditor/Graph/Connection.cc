

#include <Graph/Connection.hh>
#include <Graph/Node.hh>
#include <qpen.h>
#include <qbrush.h>

namespace graph
{
NodeConnection::NodeConnection(Node *outputNode, int outputIdx, Node *inputNode, int inputIdx, QGraphicsItem *parent)
  : QGraphicsPathItem(parent)
  , m_outputNode(outputNode)
  , m_outputIdx(outputIdx)
  , m_inputNode(inputNode)
  , m_inputIdx(inputIdx)
{
  Update();
  setPen(QPen(QBrush(QColor(255, 255, 255)), 2.0f));
}


void NodeConnection::Update()
{
  QPointF p0 = m_outputNode->GetAnchorOutputPos(m_outputIdx);
  QPointF p1 = m_inputNode->GetAnchorInputPos(m_inputIdx);


  float cx = (p0.x() + p1.x()) / 2.0f;
  QPainterPath path;
  path.moveTo(p0);
  path.cubicTo(QPointF(cx, p0.y()), QPointF(cx, p1.y()), p1);
  setPath(path);
}

}
