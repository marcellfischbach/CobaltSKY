

#include <nodegraph/nodegraphwidget.hh>
#include <nodegraph/nodegraphnode.hh>
#include <nodegraph/nodegraphnodeanchor.hh>
#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeimageproperty.hh>
#include <nodegraph/nodegraphnodeproperty.hh>
#include <nodegraph/nodegraphnodevalueproperty.hh>

#include <QPaintEvent>
#include <QPainter>

NodeGraphWidget::NodeGraphWidget(QWidget *parent)
  : QWidget(parent)
  , m_dragAnchor(0)
  , m_moveSelectedNodes(false)
  , m_armTranslation(false)
  , m_rubberBand(false)
  , m_scale(1.0f)
  , m_translate(0.0f, 0.0f)
{
  setFocusPolicy(Qt::StrongFocus);
}

NodeGraphWidget::~NodeGraphWidget()
{

}

void NodeGraphWidget::Clear()
{
  m_selectedNodes.clear();
  m_connections.clear();
  m_dragAnchor = 0;
  for (NodeGraphNode *node : m_nodes)
  {
    delete node;
  }
  m_nodes.clear();
}

void NodeGraphWidget::AddNode(NodeGraphNode *node)
{
  m_nodes.push_back(node);

  repaint();
}

void NodeGraphWidget::RemoveNode(NodeGraphNode *node)
{
  QList<NodeGraphNodeAnchor*> anchors = node->GetAllAnchors();
  for (NodeGraphNodeAnchor *anchor : anchors)
  {
    DisconnectAll(anchor);
  }
  m_nodes.removeAll(node);
  emit NodeRemoved(node);
  repaint();
}

void NodeGraphWidget::Connect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB)
{
  Connection con;
  con.m_anchorA = anchorA;
  con.m_anchorB = anchorB;
  if (!m_connections.contains(con))
  {
    emit AboutToConnect(anchorA, anchorB);
    m_connections.push_back(con);
    emit Connected(anchorA, anchorB);
    UpdateAnchorConnectionState();
    repaint();
  }

}

void NodeGraphWidget::Disconnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB)
{
  Connection con;
  con.m_anchorA = anchorA;
  con.m_anchorB = anchorB;
  if (m_connections.contains(con))
  {
    emit AboutToDisconnect(anchorA, anchorB);
    m_connections.removeAll(con);
    emit Disconnected(anchorA, anchorB);
    UpdateAnchorConnectionState();
    repaint();
  }
}

void NodeGraphWidget::DisconnectAll(NodeGraphNodeAnchor *anchor)
{
  while (true)
  {
    bool removed = false;
    for (int i = 0; i < m_connections.size(); ++i)
    {
      Connection con = m_connections[i];
      if (con.m_anchorA == anchor || con.m_anchorB == anchor)
      {
        emit AboutToDisconnect(con.m_anchorA, con.m_anchorB);
        m_connections.removeAt(i);
        emit Disconnected(con.m_anchorA, con.m_anchorB);
        removed = true;
        break;
      }
    }
    if (!removed)
    {
      break;
    }
  }
  UpdateAnchorConnectionState();
  repaint();
}

QTransform NodeGraphWidget::GetTransform() const
{
  QTransform xformAdjust;
  xformAdjust.translate(width() / 2.0f, height() / 2.0f);

  QTransform xformScale;
  xformScale.scale(m_scale, m_scale);

  QTransform xformTrans;
  xformTrans.translate(m_translate.x() + m_translateOffset.x(), m_translate.y() + m_translateOffset.y());

  return xformTrans * xformScale * xformAdjust;
}

void NodeGraphWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);


  painter.fillRect(0, 0, width(), height(), QColor(32, 40, 32));

  QTransform currentTransform = painter.transform();
  painter.setTransform(GetTransform(), false);


  for (int i = m_nodes.size() - 1; i >= 0; --i)
  {
    m_nodes[i]->paint(&painter);
  }


  for (Connection &con : m_connections)
  {
    QPointF start = con.m_anchorA->GetAbsCenter();
    QPointF end = con.m_anchorB->GetAbsCenter();
    QPainterPath path;
    path.moveTo(start);
    float mid = (start.x() + end.x()) / 2.0f;
    path.cubicTo(mid, start.y(), mid, end.y(), end.x(), end.y());
    painter.setPen(QPen(QColor(255, 255, 255, 255)));
    painter.drawPath(path);

  }

  if (m_dragAnchor)
  {
    QPointF start = m_dragAnchor->GetAbsCenter();
    NodeGraphNodeAnchor *destAnchor = GetAnchor(m_dragPos);
    QPointF end = m_dragPos;
    if (destAnchor)
    {
      NodeGraphVetoEvent veto;
      emit CheckConnection(m_dragAnchor, destAnchor, &veto);
      if (!veto.IsVeto())
      {
        end = destAnchor->GetAbsCenter();
      }
    }

    QPainterPath path;
    path.moveTo(start);
    float mid = (start.x() + end.x()) / 2.0f;
    path.cubicTo(mid, start.y(), mid, end.y(), end.x(), end.y());
    QVector<qreal> dash;
    dash << 10 << 10;
    QPen pen(QColor(255, 255, 255, 255));
    pen.setDashPattern(dash);
    painter.setPen(pen);
    painter.drawPath(path);
  }

  painter.setTransform(currentTransform);
  if (m_rubberBand)
  {
    QVector<qreal> dash;
    dash << 10 << 10;
    QPen pen(QColor(255, 255, 255, 255));
    pen.setDashPattern(dash);
    painter.setPen(pen);
    //painter.drawRect(QRect(unmap(m_dragStart), unmap(m_dragPos)));
    painter.fillRect(QRect(unmap(m_dragStart), unmap(m_dragPos)), QColor(255, 255, 255, 64));
  }


}

void NodeGraphWidget::ClearSelection(bool emitSignal)
{
  for (NodeGraphNode *node : m_selectedNodes)
  {
    node->SetSelected(false);
  }
  m_selectedNodes.clear();
  if (emitSignal)
  {
    emit SelectionChanged(m_selectedNodes);
  }
}

void NodeGraphWidget::mousePressEvent(QMouseEvent *event)
{
  bool addSelection = AddSelection(event);
  QPointF p = map(QPoint(event->x(), event->y()));
  if (event->button() == Qt::LeftButton)
  {

    m_dragAnchor = 0;
    m_dragStart = p;
    m_dragPos = p;
    m_moveSelectedNodes = false;
    for (NodeGraphNode *node : m_nodes)
    {
      if (node->TestHandle(p))
      {
        if (!m_selectedNodes.contains(node))
        {
          if (!addSelection)
          {
            ClearSelection(false);
          }
          m_selectedNodes.push_back(node);
          emit SelectionChanged(m_selectedNodes);
          node->SetSelected(true);
          SortNodesBySelection();
        }

        m_moveSelectedNodes = true;
        return;
      }
      NodeGraphNodeAnchor *anchor = node->GetAnchor(p);
      if (anchor)
      {
        ClearSelection();
        m_dragAnchor = anchor;
        return;
      }
    }


    if (!addSelection)
    {
      ClearSelection();
    }
    m_rubberBand = true;
  }
  else if (event->button() == Qt::RightButton)
  {
    m_armTranslation = true;
    m_transStart = event->pos();
    m_transPos = event->pos();
  }
  repaint();
}

void NodeGraphWidget::mouseMoveEvent(QMouseEvent *event)
{
  QPointF p = map(QPoint(event->x(), event->y()));  
  m_dragPos = p;
  if (!m_selectedNodes.isEmpty() && m_moveSelectedNodes)
  {
    for (NodeGraphNode *node : m_selectedNodes)
    {
      node->SetOffset(m_dragPos - m_dragStart);
    }
  }

  if (m_armTranslation)
  {
    m_transPos = event->pos();
    m_translateOffset = (m_transPos - m_transStart) / m_scale;
  }

  if (!m_selectedNodes.isEmpty() || m_dragAnchor || m_rubberBand || m_armTranslation)
  {
    repaint();
  }
  
}

void NodeGraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
  QPointF p = map(QPoint(event->x(), event->y()));
  if (event->button() == Qt::LeftButton)
  {
    m_dragPos = p;
    m_moveSelectedNodes = false;

    if (m_dragAnchor)
    {
      NodeGraphNodeAnchor *endAnchor = GetAnchor(p);
      if (endAnchor)
      {
        NodeGraphVetoEvent veto;
        emit CheckConnection(m_dragAnchor, endAnchor, &veto);
        if (!veto.IsVeto())
        {
          Connect(m_dragAnchor, endAnchor);
        }
      }
      else
      {
        NodeGraphVetoEvent veto;
        emit CheckLooseDisconnect(m_dragAnchor, &veto);
        if (!veto.IsVeto())
        {
          DisconnectAll(m_dragAnchor);
        }
      }
    }
    if (!m_selectedNodes.isEmpty())
    {
      for (NodeGraphNode *node : m_selectedNodes)
      {
        node->CommitOffset();
      }
    }
    if (m_rubberBand)
    {
      if (!AddSelection(event))
      {
        ClearSelection();
      }
      SelectByRubberBand();
      m_rubberBand = false;
    }

    m_dragAnchor = 0;

    repaint();
  }
  if (event->button() == Qt::RightButton)
  { 
    m_armTranslation = false;
    m_translate += m_translateOffset;
    m_translateOffset = QPointF(0, 0);
  }
}

void NodeGraphWidget::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Delete)
  {
    QList<NodeGraphNode*> nodes(m_selectedNodes);
    NodeGraphVetoEvent veto;
    emit RequestRemoveNode(nodes, &veto);
    if (veto.IsVeto())
    {
      return;
    }
    m_selectedNodes.clear();
    for (NodeGraphNode *node : nodes)
    {
      RemoveNode(node);
      delete node;
    }
    nodes.clear();
    emit SelectionChanged(m_selectedNodes);
  }
}

void NodeGraphWidget::wheelEvent(QWheelEvent *event)
{
  int delta = event->angleDelta().y();
  if (delta < 0)
  {
    if (m_scale > 0.1f)
    {
      m_scale *= 0.9f;
    }
  }
  else if (delta > 0)
  {
    m_scale /= 0.9f;
    if (m_scale > 1.0f)
    {
      m_scale = 1.0f;
    }
  }
  else
  {
    return;
  }
  emit ScaleChanged(m_scale);
  repaint();
}


void NodeGraphWidget::dragEnterEvent(QDragEnterEvent *event)
{
  NodeGraphAcceptEvent accept;
  emit CheckDrag(event, &accept);
  if (accept.IsAccepted())
  {
    event->accept();
  }
}

void NodeGraphWidget::dragMoveEvent(QDragMoveEvent *event)
{
  NodeGraphAcceptEvent accept;
  emit CheckDrag(event, &accept);
  if (accept.IsAccepted())
  {
    event->accept();
  }
}

void NodeGraphWidget::dropEvent(QDropEvent *event)
{
  NodeGraphAcceptEvent accept;
  emit CheckDrag(event, &accept);
  if (accept.IsAccepted())
  {
    emit DragDropped(event);
    event->accept();
  }
}

void NodeGraphWidget::SelectByRubberBand()
{
  QRectF rect(m_dragStart, m_dragPos);
  for (NodeGraphNode *node : m_nodes)
  {
    if (rect.contains(node->GetBounding()))
    {
      node->SetSelected(true);
      m_selectedNodes.push_back(node);
      emit SelectionChanged(m_selectedNodes);
    }
  }
}

QPointF NodeGraphWidget::map(const QPoint &point) const
{
  QTransform t = GetTransform();
  t = t.inverted();

  return t.map(QPointF(point.x(), point.y()));
}

QPoint NodeGraphWidget::unmap(const QPointF &point) const
{
  QTransform t = GetTransform();

  QPointF p = t.map(QPointF(point.x(), point.y()));
  return QPoint(p.x(), p.y());
}

bool NodeGraphWidget::AddSelection(QMouseEvent *event) const
{
  return (event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier)) != 0;
}

namespace
{
bool NodesSortLess(NodeGraphNode *n0, NodeGraphNode *n1)
{
  return n0->IsSelected() && !n1->IsSelected();
}
}

void NodeGraphWidget::SortNodesBySelection()
{
  qSort(m_nodes.begin(), m_nodes.end(), ::NodesSortLess);
}

void NodeGraphWidget::UpdateAnchorConnectionState()
{
  for (NodeGraphNode *node : m_nodes)
  {
    node->SetAllAnchorsDisconnected();
  }
  for (Connection &con : m_connections)
  {
    con.m_anchorA->SetConnected(true);
    con.m_anchorB->SetConnected(true);
  }
  for (NodeGraphNode *node : m_nodes)
  {
    node->Layout();
  }
}

NodeGraphNodeAnchor *NodeGraphWidget::GetAnchor(const QPointF &point) const
{
  for (const NodeGraphNode *node : m_nodes)
  {
    NodeGraphNodeAnchor *anchor = node->GetAnchor(point);
    if (anchor)
    {
      return anchor;
    }
  }
  return 0;
}


bool NodeGraphWidget::Connection::operator==(const NodeGraphWidget::Connection &other) const
{
  return m_anchorA == other.m_anchorA && m_anchorB == other.m_anchorB
    || m_anchorA == other.m_anchorB && m_anchorB == other.m_anchorA;
}




QPointF NodeGraphWidget::GetLocalCoordinate(const QPoint &screenCoordinate)
{
  return map(screenCoordinate);
}

