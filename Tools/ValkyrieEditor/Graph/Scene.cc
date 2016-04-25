

#include <Graph/Scene.hh>
#include <Graph/Connection.hh>
#include <Graph/Node.hh>

namespace graph
{

NodeGraphScene::NodeGraphScene(QObject *parent)
  : QGraphicsScene(parent)
  , m_currentConnectionPath(0)
{
  setSceneRect(QRect(-2000, -2000, 4000, 4000));
}


void NodeGraphScene::AddNode(Node *node)
{
  m_nodes.append(node);
  node->SetScene(this);
  addItem(node->GetItem());

  emit NodeAdded(node);
}

void NodeGraphScene::RemoveSelectedNode()
{
  Node *selected = Node::GetSelected();
  if (!selected)
  {
    return;
  }

  Node::Select(0);
  m_nodes.removeAll(selected);
  selected->SetScene(0);
  selected->RemoveAllConnections();
  removeItem(selected->GetItem());

  QList<NodeConnection*> removes;
  for (auto connection : m_connections)
  {
    if (connection->GetInputNode() == selected || connection->GetOutputNode() == selected)
    {
      removes.append(connection);
    }
  }
  for (auto connection : removes)
  {
    m_connections.removeAll(connection);
    delete connection;
  }


  emit NodeRemoved(selected);
  delete selected;
}

void NodeGraphScene::Connect(Node *nodeOutput, int outputIdx, Node *nodeInput, int inputIdx)
{
  NodeConnection *connection = new NodeConnection(nodeOutput, outputIdx, nodeInput, inputIdx, 0);
  addItem(connection);
  m_connections.append(connection);

  emit NodesConnected(nodeOutput, outputIdx, nodeInput, inputIdx);
  ResetConstValues();
}


void NodeGraphScene::Disconnect(NodeConnection *connection, bool resetConst)
{
  removeItem(connection);
  m_connections.removeAll(connection);

  emit NodesDisconnected(connection->GetOutputNode(),
                         connection->GetOutputIdx(),
                         connection->GetInputNode(),
                         connection->GetInputIdx());
  delete connection;
  if (resetConst)
  {
    ResetConstValues();
  }
}

void NodeGraphScene::Disconnect(Node *nodeOutput, int outputIdx, Node *nodeInput, int inputIdx)
{

  for (NodeConnection *connection : m_connections)
  {
    if (connection->GetOutputNode() == nodeOutput &&
        connection->GetOutputIdx() == outputIdx &&
        connection->GetInputNode() == nodeInput &&
        connection->GetInputIdx() == inputIdx)
    {
      Disconnect(connection);
      return;
    }
  }
}

void NodeGraphScene::DisconnectInput(Node *nodeInput, int inputIdx)
{

  bool disconnected = false;
  for (NodeConnection *connection : m_connections)
  {
    if (connection->GetInputNode() == nodeInput &&
        connection->GetInputIdx() == inputIdx)
    {
      Disconnect(connection, false);
      disconnected = true;
    }
  }

  if (disconnected)
  {
    ResetConstValues();
  }
}

void NodeGraphScene::DisconnectOutput(Node *nodeOutput, int outputIdx)
{

  bool disconnected = false;
  for (NodeConnection *connection : m_connections)
  {
    if (connection->GetOutputNode() == nodeOutput &&
        connection->GetOutputIdx() == outputIdx)
    {
      Disconnect(connection, false);
      disconnected = true;
    }
  }

  if (disconnected)
  {
    ResetConstValues();
  }
}

size_t NodeGraphScene::GetNumberOfNodes()
{
  return m_nodes.size();
}

Node *NodeGraphScene::GetNode(size_t idx)
{
  if (idx >= m_nodes.size())
  {
    return 0;
  }
  return m_nodes[idx];
}

size_t NodeGraphScene::GetNumberOfConnections()
{
  return m_connections.size();
}

NodeConnection *NodeGraphScene::GetConnection(size_t idx)
{
  if (idx >= m_connections.size())
  {
    return 0;
  }
  return m_connections[idx];
}




void NodeGraphScene::ResetConstValues()
{
  for (Node *node : m_nodes)
  {
    node->ResetConstVisible();
  }
  for (NodeConnection *connection : m_connections)
  {
    connection->GetInputNode()->SetConstVisible(connection->GetInputIdx(), false);
  }
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

  switch (dir)
  {
  case eD_Input:
    emit NodeConnectedLooseInput(node, idx);
    break;
  case eD_Output:
    emit NodeConnectedLooseOutput(node, idx);
    break;
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