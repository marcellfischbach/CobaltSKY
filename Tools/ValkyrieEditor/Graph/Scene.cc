

#include <Graph/Scene.hh>
#include <Graph/Connection.hh>
#include <Graph/Node.hh>

namespace graph
{

NodeGraphScene::NodeGraphScene(QObject *parent)
  : QGraphicsScene(parent)
  , m_currentConnectionPath(0)
{

}


void NodeGraphScene::AddNode(Node *node)
{
  m_nodes.append(node);
  node->SetScene(this);
  addItem(node->GetItem());

  emit NodeAdded(node);
}

void NodeGraphScene::Connect(Node *nodeOutput, int outputIdx, Node *nodeInput, int inputIdx)
{
  NodeConnection *connection = new NodeConnection(nodeOutput, outputIdx, nodeInput, inputIdx, 0);
  addItem(connection);
  m_connections.append(connection);

  emit NodesConnected(nodeOutput, outputIdx, nodeInput, inputIdx);
}

void NodeGraphScene::NodeMoved(Node *node)
{
  for (NodeConnection *connection : m_connections)
  {
    if (connection->GetInputNode() == node || connection->GetOutputNode() == node)
    {
      connection->Update();
    }
  }
}

void NodeGraphScene::MoveConnection(Node *node, int idx, Direction dir, const QPointF &p0, const QPointF &p1)
{
  QGraphicsPathItem *path = GetCurrentConnectionPath();
  QPainterPath ppath;

  QPointF p2 = TestForAnchorMatch(p1, node, dir);

  float mx = (p0.x() + p2.x()) / 2.0f;
  ppath.moveTo(p0);
  ppath.cubicTo(QPointF(mx, p0.y()), QPointF(mx, p2.y()), p2);
  path->setPath(ppath);
}


void NodeGraphScene::StopConnection(Node *node, int idx, Direction dir, const QPointF &p0, const QPointF &p1)
{
  removeItem(m_currentConnectionPath);
  delete m_currentConnectionPath;
  m_currentConnectionPath = 0;

  Node::AnchorRequestResult result;
  for (Node *n : m_nodes)
  {
    if (n != node && n->TestAnchor(p1, result))
    {
      if (result.dir != dir)
      {
        switch (dir)
        {
        case eD_Input:
          Connect(n, result.idx, node, idx);
          break;
        case eD_Output:
          Connect(node, idx, n, result.idx);
          break;
        }
        return;
      }
    }
  }

}

QGraphicsPathItem *NodeGraphScene::GetCurrentConnectionPath()
{
  if (!m_currentConnectionPath)
  {
    m_currentConnectionPath = new QGraphicsPathItem();
    m_currentConnectionPath->setPen(QPen(QBrush(QColor(255, 255, 255)), 2.0f));

    addItem(m_currentConnectionPath);
  }

  return m_currentConnectionPath;
}


QPointF NodeGraphScene::TestForAnchorMatch(const QPointF &p, Node *node, Direction dir)
{
  Node::AnchorRequestResult result;
  for (Node *n : m_nodes)
  {
    if (n != node && n->TestAnchor(p, result))
    {
      if (result.dir != dir)
      {
        return result.pos;
      }
    }
  }
  return p;
}

}