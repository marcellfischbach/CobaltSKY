

#include <Graph/Scene.hh>
#include <Graph/Connection.hh>
#include <Graph/Node.hh>
#include <QGraphicsSceneDragDropEvent>
#include <QPainterPath>

namespace graph
{

NodeGraphScene::NodeGraphScene(QObject *parent)
  : QGraphicsScene(parent)
  , m_currentConnectionPath(0)
  , m_currentSelectedNode(0)
  , m_silent (false)
  , m_currentAnchorWidget(0)
{
  setSceneRect(QRect(-2000, -2000, 4000, 4000));

  m_currentConnectionPath = new QGraphicsPathItem();
  m_currentConnectionPath->setPen(QPen(QColor(255, 255, 255), 2.0f));
  addItem(m_currentConnectionPath);
}

void NodeGraphScene::SetSilent(bool silent)
{
  m_silent = silent;
}

bool NodeGraphScene::IsSilent() const
{
  return m_silent;
}

void NodeGraphScene::AddNode(Node *node)
{
  m_nodes.append(node);
  node->SetScene(this);
  //addItem(node->GetItem());

  GraphNode *gn = new GraphNode();
  m_graphNodes.append(gn);
//  gn->setGeometry(0, 0, 200, 100);
  addItem (gn);

  if (!m_silent)
  {
    emit NodeAdded(node);
  }
}

void NodeGraphScene::RemoveSelectedNode()
{
  Node *selected = m_currentSelectedNode;
  if (!selected)
  {
    return;
  }

  m_currentSelectedNode = 0;
  EmitCurrentNodeChanged ();
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
    removeItem(connection);
    delete connection;
  }


  if (!m_silent)
  {
    emit NodeRemoved(selected);
  }
  delete selected;
}

void NodeGraphScene::Connect(Node *nodeOutput, int outputIdx, Node *nodeInput, int inputIdx)
{
  NodeConnection *connection = new NodeConnection(nodeOutput, outputIdx, nodeInput, inputIdx, 0);
  addItem(connection);
  m_connections.append(connection);

  if (!m_silent)
  {
    emit NodesConnected(nodeOutput, outputIdx, nodeInput, inputIdx);
  }
  ResetConstValues();
}


void NodeGraphScene::Disconnect(NodeConnection *connection, bool resetConst)
{
  removeItem(connection);
  m_connections.removeAll(connection);

  if (!m_silent)
  {
    emit NodesDisconnected(connection->GetOutputNode(),
                           connection->GetOutputIdx(),
                           connection->GetInputNode(),
                           connection->GetInputIdx());
  }
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


void NodeGraphScene::SelectNode(graph::Node *node)
{
  graph::Node *current = m_currentSelectedNode;
  m_currentSelectedNode = node;

  if (current == node)
  {
    return;
  }

  if (current)
  {
    current->UpdateSelection();
  }
  if (node)
  {
    node->UpdateSelection();
  }

  EmitCurrentNodeChanged();
}

graph::Node *NodeGraphScene::GetSelectedNode()
{
  return m_currentSelectedNode;
}

const graph::Node *NodeGraphScene::GetSelectedNode() const
{
  return m_currentSelectedNode;
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
    if (!m_silent)
    {
      emit NodeConnectedLooseInput(node, idx);
    }
    break;
  case eD_Output:
    if (!m_silent)
    {
      emit NodeConnectedLooseOutput(node, idx);
    }
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

void NodeGraphScene::EmitCurrentNodeChanged()
{
  if (!m_silent)
  {
    emit NodeSelected(m_currentSelectedNode);
  }
}


void NodeGraphScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsScene::mousePressEvent(event);

  QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
  if (!item && !event->modifiers().testFlag(Qt::ShiftModifier) && !event->modifiers().testFlag(Qt::ControlModifier))
  {
    ClearSelection();
  }

}


void NodeGraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsScene::mouseMoveEvent(event);
  for (auto node : m_graphNodes)
  {
    node->Hover(event->scenePos());
  }
}



void NodeGraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsScene::mouseReleaseEvent(event);
}


void NodeGraphScene::StartConnection(AnchorWidget *item)
{
  m_currentAnchorWidget = item;
}

void NodeGraphScene::MoveConnection(const QPointF &scenePos)
{
  QPointF endPos = scenePos;
  if (!m_currentAnchorWidget)
  {
    return;
  }

  QPointF startPos = m_currentAnchorWidget->mapToScene(m_currentAnchorWidget->contentsRect().center());

  AnchorWidget *endItem = FindAnchorWidget(scenePos);
  if (IsConnectable(m_currentAnchorWidget, endItem))
  {
    endPos = endItem->mapToScene(endItem->contentsRect().center());
  }

  QPainterPath path;
  path.moveTo(startPos);

  float cx = (startPos.x() + endPos.x()) / 2.0f;
  float cy0 = startPos.y();
  float cy1 = endPos.y();
  path.cubicTo(QPointF(cx, cy0), QPointF(cx, cy1), endPos);
  m_currentConnectionPath->setPath(path);
}

void NodeGraphScene::AttachConnect(const QPointF &scenePos)
{
  QPainterPath path;
  m_currentConnectionPath->setPath(path);

  AnchorWidget *startItem = m_currentAnchorWidget;
  m_currentAnchorWidget = 0;

  AnchorWidget *endItem = FindAnchorWidget(scenePos);
  if (!IsConnectable(startItem, endItem))
  {
    OnConnectionCanceled(startItem);
    return;
  }

  AddConnection(startItem, endItem);
  m_currentAnchorWidget = 0;
}

AnchorConnectionItem *NodeGraphScene::AddConnection (AnchorWidget *anchorA, AnchorWidget *anchorB)
{
  AnchorConnectionItem *anchorConnection = new AnchorConnectionItem(0, anchorA, anchorB);
  addItem(anchorConnection);
  m_connectionItems.append(anchorConnection);
  OnConnectionAdded(anchorConnection);
  return anchorConnection;
}

bool NodeGraphScene::RemoveConnection(AnchorWidget *anchorA, AnchorWidget *anchorB)
{
  for (auto connection : m_connectionItems)
  {
    if (connection->IsEqual(anchorA, anchorB))
    {
      return RemoveConnection(connection);
    }
  }
  return false;
}

bool NodeGraphScene::RemoveConnection(AnchorConnectionItem *connection)
{
  int numRemoved = m_connectionItems.removeAll(connection);
  removeItem(connection);
  OnConnectionRemoved(connection);
  delete connection;
  return numRemoved;
}

bool NodeGraphScene::IsConnectable(AnchorWidget *anchorA, AnchorWidget *anchorB) const
{
  if (!anchorA || !anchorB)
  {
    return false;
  }

  return anchorA->GetType() == anchorB->GetType() && anchorA->GetDirection() != anchorB->GetDirection();
}



void NodeGraphScene::ClearSelection ()
{
  for (auto node : m_graphNodes)
  {
    node->SetSelected(false);
  }
}


void NodeGraphScene::SelectNode(GraphNode *node, Qt::KeyboardModifiers modifiers)
{
  if (!modifiers.testFlag(Qt::ShiftModifier) && !modifiers.testFlag(Qt::ControlModifier))
  {
    ClearSelection ();
  }
  node->SetSelected(true);
}


void NodeGraphScene::StartMoving()
{
  for (auto node : m_graphNodes)
  {
    node->SetMotionStart(node->pos());
  }

}

void NodeGraphScene::MoveSelectedNodes(const QPointF &direction)
{
  for (auto node : m_graphNodes)
  {
    if (node->IsSelected())
    {
      node->setPos(node->GetMotionStart() + direction);
    }
  }
}

AnchorWidget *NodeGraphScene::FindAnchorWidget(const QPointF &scenePos)
{
  for (auto node : m_graphNodes)
  {
    AnchorWidget *item = node->FindAnchorWidget(scenePos);
    if (item)
    {
      return item;
    }
  }
  return 0;
}


}
