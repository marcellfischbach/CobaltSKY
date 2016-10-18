

#include <Graph/Scene.hh>
#include <Graph/Node.hh>
#include <QGraphicsSceneDragDropEvent>
#include <QPainterPath>

namespace graph
{

NodeGraphScene::NodeGraphScene(QObject *parent)
  : QGraphicsScene(parent)
  , m_currentConnectionPath(0)
  , m_currentAnchorWidget(0)
{
  setSceneRect(QRect(-2000, -2000, 4000, 4000));

  m_currentConnectionPath = new QGraphicsPathItem();
  m_currentConnectionPath->setPen(QPen(QColor(255, 255, 255), 2.0f));
  addItem(m_currentConnectionPath);
}


void NodeGraphScene::Clear ()
{
  removeItem(m_currentConnectionPath);
  clear();
  addItem(m_currentConnectionPath);
}

void NodeGraphScene::AddNode(GraphNode *node)
{

  m_graphNodes.append(node);
  addItem (node);

}

void NodeGraphScene::RemoveSelectedNodes()
{

  QList<GraphNode*> toBeRemoved;
  for (auto node : m_graphNodes)
  {
    if (node->IsSelected())
    {
      toBeRemoved.append(node);
    }
  }

  ClearSelection();
  for (auto node : toBeRemoved)
  {
    RemoveNode(node);
  }
}

void NodeGraphScene::RemoveNode(GraphNode *node)
{
  QList<AnchorConnectionItem*> toBeRemoved;
  for (auto connection : m_connectionItems)
  {
    if (connection->GetAnchorA() && connection->GetAnchorA()->GetGraphNode() == node ||
        connection->GetAnchorB() && connection->GetAnchorB()->GetGraphNode() == node)
    {
      toBeRemoved.append(connection);
    }
  }

  for (auto connection : toBeRemoved)
  {
    RemoveConnection(connection);
  }

  removeItem(node);
  OnNodeRemoved(node);
}


size_t NodeGraphScene::GetNumberOfNodes()
{
  return m_graphNodes.size();
}

GraphNode *NodeGraphScene::GetNode(size_t idx)
{
  if (idx >= m_graphNodes.size())
  {
    return 0;
  }
  return m_graphNodes[(int)idx];
}

size_t NodeGraphScene::GetNumberOfConnections()
{
  return m_connectionItems.size();
}

AnchorConnectionItem *NodeGraphScene::GetConnection(size_t idx)
{
  if (idx >= m_connectionItems.size())
  {
    return 0;
  }
  return m_connectionItems[(int)idx];
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

void NodeGraphScene::OnConnectionAdded(AnchorConnectionItem *connection)
{
  emit ConnectionAdded(connection);
}

void NodeGraphScene::OnConnectionCanceled(AnchorWidget *anchor)
{

}

void NodeGraphScene::OnConnectionRemoved(AnchorConnectionItem *connection)
{
  emit ConnectionRemoved (connection);
}

void NodeGraphScene::OnNodeAdded(GraphNode *node)
{
  emit NodeAdded(node);
}

void NodeGraphScene::OnNodeRemoved(GraphNode *node)
{
  emit NodeRemoved(node);
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
