

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
  , m_rubberBand(false)
  , m_scale(1.0f)
  , m_translate(0.0f, 0.0f)
{
  NodeGraphNode *node0 = new NodeGraphNode();
  node0->SetLocation(QPointF(100.0f, 100.0f));
  node0->GetHeader()->SetInShow(false);
  node0->GetHeader()->SetOutShow(false);
  node0->GetHeader()->SetName("Texture2D");

  NodeGraphNodeImageProperty *propI = new NodeGraphNodeImageProperty(node0);
  propI->SetImage(QImage(":/icons/resources/UnknownAsset64.png"));
  node0->AddInputProperty(propI);

  NodeGraphNodeValueProperty *prop0 = new NodeGraphNodeValueProperty(node0);
  prop0->SetName("uv");
  prop0->SetAnchorShow(true);
  prop0->SetShowValue(false);
  node0->AddInputProperty(prop0);
  node0->Layout();

  NodeGraphNodeValueProperty *prop1 = new NodeGraphNodeValueProperty(node0);
  prop1->SetName("C");
  prop1->SetAnchorShow(true);
  prop1->SetShowValue(false);
  node0->AddOutputProperty(prop1);

  NodeGraphNodeValueProperty *prop2 = new NodeGraphNodeValueProperty(node0);
  prop2->SetName("R");
  prop2->SetAnchorColor(QColor(255, 0, 0, 255));
  prop2->SetTextColor(QColor(255, 0, 0, 255));
  prop2->SetAnchorShow(true);
  prop2->SetShowValue(false);
  node0->AddOutputProperty(prop2);

  NodeGraphNodeValueProperty *prop3 = new NodeGraphNodeValueProperty(node0);
  prop3->SetName("G");
  prop3->SetAnchorColor(QColor(0, 255, 0, 255));
  prop3->SetTextColor(QColor(0, 255, 0, 255));
  prop3->SetAnchorShow(true);
  prop3->SetShowValue(false);
  node0->AddOutputProperty(prop3);

  NodeGraphNodeValueProperty *prop4 = new NodeGraphNodeValueProperty(node0);
  prop4->SetName("B");
  prop4->SetAnchorColor(QColor(0, 0, 255, 255));
  prop4->SetTextColor(QColor(0, 0, 255, 255));
  prop4->SetAnchorShow(true);
  prop4->SetShowValue(false);
  node0->AddOutputProperty(prop4);

  NodeGraphNodeValueProperty *prop5 = new NodeGraphNodeValueProperty(node0);
  prop5->SetName("A");
  prop5->SetAnchorShow(true);
  prop5->SetShowValue(false);
  node0->AddOutputProperty(prop5);

  node0->Layout();


  AddNode(node0);


  NodeGraphNode *node1 = new NodeGraphNode();
  node1->SetLocation(QPointF(400.0f, 100.0f));
  node1->GetHeader()->SetName("Add");

  NodeGraphNodeValueProperty *propA = new NodeGraphNodeValueProperty(node1);
  propA->SetAnchorShow(true);
  propA->SetShowValue(true);
  propA->SetName("A");
  propA->SetValue(1.0f);
  node1->AddInputProperty(propA);

  NodeGraphNodeValueProperty *propB = new NodeGraphNodeValueProperty(node1);
  propB->SetAnchorShow(true);
  propB->SetShowValue(true);
  propB->SetName("B");
  propB->SetValue(1.0f);
  node1->AddInputProperty(propB);

  NodeGraphNodeValueProperty *propC = new NodeGraphNodeValueProperty(node1);
  propC->SetAnchorShow(true);
  propC->SetName("X");
  node1->AddOutputProperty(propC);

  node1->Layout();
  AddNode(node1);
}

NodeGraphWidget::~NodeGraphWidget()
{

}


void NodeGraphWidget::AddNode(NodeGraphNode *node)
{
  m_nodes.push_back(node);

  repaint();
}

void NodeGraphWidget::RemoveNode(NodeGraphNode *node)
{
  m_nodes.removeAll(node);

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


  painter.fillRect(0, 0, width(), height(), QColor(255, 255, 255));

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
    painter.setPen(QPen(QColor(0, 0, 0)));
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
    QPen pen(QColor(0, 0, 0, 255));
    pen.setDashPattern(dash);
    painter.setPen(pen);
    painter.drawPath(path);
  }

  painter.setTransform(currentTransform);
  if (m_rubberBand)
  {
    QVector<qreal> dash;
    dash << 10 << 10;
    QPen pen(QColor(0, 0, 0, 255));
    pen.setDashPattern(dash);
    painter.setPen(pen);
    painter.drawRect(QRect(unmap(m_dragStart), unmap(m_dragPos)));
  }


}

void NodeGraphWidget::ClearSelection()
{
  for (NodeGraphNode *node : m_selectedNodes)
  {
    node->SetSelected(false);
  }
  m_selectedNodes.clear();
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
            ClearSelection();
          }
          m_selectedNodes.push_back(node);
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
  else if (event->button() == Qt::MiddleButton)
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
  if (event->button() == Qt::MiddleButton)
  { 
    m_armTranslation = false;
    m_translate += m_translateOffset;
    m_translateOffset = QPointF(0, 0);
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
  }
  else
  {
    return;
  }
  emit ScaleChanged(m_scale);
  repaint();
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


