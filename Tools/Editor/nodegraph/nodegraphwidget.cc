

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
  , m_dragNode(0)
{
  NodeGraphNode *node0 = new NodeGraphNode();
  node0->SetLocation(100, 100);
  node0->GetHeader()->SetInShow(false);
  node0->GetHeader()->SetOutShow(false);
  node0->GetHeader()->SetName("Texture2D");

  NodeGraphNodeImageProperty *propI = new NodeGraphNodeImageProperty();
  propI->SetImage(QImage(":/icons/resources/UnknownAsset64.png"));
  node0->AddInputProperty(propI);

  NodeGraphNodeValueProperty *prop0 = new NodeGraphNodeValueProperty();
  prop0->SetName("uv");
  prop0->SetAnchorShow(true);
  prop0->SetAnchorConnected(true);
  prop0->SetShowValue(false);
  node0->AddInputProperty(prop0);
  node0->Layout();

  NodeGraphNodeValueProperty *prop1 = new NodeGraphNodeValueProperty();
  prop1->SetName("C");
  prop1->SetAnchorShow(true);
  prop1->SetAnchorConnected(true);
  prop1->SetShowValue(false);
  node0->AddOutputProperty(prop1);

  NodeGraphNodeValueProperty *prop2 = new NodeGraphNodeValueProperty();
  prop2->SetName("R");
  prop2->SetAnchorColor(QColor(255, 0, 0, 255));
  prop2->SetTextColor(QColor(255, 0, 0, 255));
  prop2->SetAnchorShow(true);
  prop2->SetAnchorConnected(true);
  prop2->SetShowValue(false);
  node0->AddOutputProperty(prop2);

  NodeGraphNodeValueProperty *prop3 = new NodeGraphNodeValueProperty();
  prop3->SetName("G");
  prop3->SetAnchorColor(QColor(0, 255, 0, 255));
  prop3->SetTextColor(QColor(0, 255, 0, 255));
  prop3->SetAnchorShow(true);
  prop3->SetAnchorConnected(true);
  prop3->SetShowValue(false);
  node0->AddOutputProperty(prop3);

  NodeGraphNodeValueProperty *prop4 = new NodeGraphNodeValueProperty();
  prop4->SetName("B");
  prop4->SetAnchorColor(QColor(0, 0, 255, 255));
  prop4->SetTextColor(QColor(0, 0, 255, 255));
  prop4->SetAnchorShow(true);
  prop4->SetAnchorConnected(true);
  prop4->SetShowValue(false);
  node0->AddOutputProperty(prop4);

  NodeGraphNodeValueProperty *prop5 = new NodeGraphNodeValueProperty();
  prop5->SetName("A");
  prop5->SetAnchorShow(true);
  prop5->SetAnchorConnected(true);
  prop5->SetShowValue(false);
  node0->AddOutputProperty(prop5);

  node0->Layout();


  AddNode(node0);


  NodeGraphNode *node1 = new NodeGraphNode();
  node1->SetLocation(400, 100);
  node1->GetHeader()->SetName("Add");

  NodeGraphNodeValueProperty *propA = new NodeGraphNodeValueProperty();
  propA->SetAnchorShow(true);
  propA->SetShowValue(true);
  propA->SetName("A");
  propA->SetValue(1.0f);
  node1->AddInputProperty(propA);

  NodeGraphNodeValueProperty *propB = new NodeGraphNodeValueProperty();
  propB->SetAnchorShow(true);
  propB->SetShowValue(true);
  propB->SetName("B");
  propB->SetValue(1.0f);
  node1->AddInputProperty(propB);

  NodeGraphNodeValueProperty *propC = new NodeGraphNodeValueProperty();
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



void NodeGraphWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);


  painter.fillRect(0, 0, width(), height(), QColor(200, 200, 200));

  QTransform currentTransform = painter.transform();
  QTransform scaleTransform;
  scaleTransform.scale(1, 1);
  painter.setTransform(scaleTransform, true);


  for (NodeGraphNode *node : m_nodes)
  {
    node->paint(&painter);
  }


  if (m_dragAnchor && m_dragAnchorNode)
  {
    QPointF start = m_dragAnchorNode->GetBounding().topLeft() + m_dragAnchor->GetBounds().center();
    QPainterPath path;
    path.moveTo(start);
    float mid = (start.x() + m_dragPos.x()) / 2.0f;
    path.cubicTo(mid, start.y(), mid, m_dragPos.y(), m_dragPos.x(), m_dragPos.y());
    painter.setPen(QPen(QColor(255, 255, 255)));
    painter.drawPath(path);
  }

  painter.setTransform(currentTransform);
}

void NodeGraphWidget::mousePressEvent(QMouseEvent *event)
{
  QPointF p = map(QPoint(event->x(), event->y()));
  m_dragNode = 0;
  m_dragAnchor = 0;
  m_dragAnchorNode = 0;

  for (NodeGraphNode *node : m_nodes)
  {
    if (node->TestHandle(p))
    {
      m_dragNode = node;
      printf("Start Draging Node\n");
      return;
    }
    NodeGraphNodeAnchor *anchor = node->GetAnchor(p);
    if (anchor)
    {
      m_dragAnchor = anchor;
      m_dragAnchorNode = node;
      printf("Start Draging Anchor\n");
      return;
    }
  }
  repaint();
}

void NodeGraphWidget::mouseMoveEvent(QMouseEvent *event)
{
  QPointF p = map(QPoint(event->x(), event->y()));  
  m_dragPos = p;
  if (m_dragNode || m_dragAnchor)
  {
    repaint();
  }
}

void NodeGraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
  QPointF p = map(QPoint(event->x(), event->y()));
  m_dragPos = p;
  m_dragNode = 0;
  m_dragAnchor = 0;
  m_dragAnchorNode = 0;

  repaint();
}

QPointF NodeGraphWidget::map(const QPoint &point) const
{
  return QPointF(point.x(), point.y());
}
