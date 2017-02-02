

#include <nodegraph/nodegraphwidget.hh>
#include <nodegraph/nodegraphheader.hh>
#include <nodegraph/nodegraphnode.hh>

#include <QPaintEvent>
#include <QPainter>

NodeGraphWidget::NodeGraphWidget(QWidget *parent)
  : QWidget(parent)
{
  NodeGraphNode *node0 = new NodeGraphNode();
  node0->SetBounding(QRect(100, 100, 200, 100));
  node0->GetHeader()->SetInShow(true);
  node0->GetHeader()->SetOutShow(true);
  node0->GetHeader()->SetName("Node0");
  node0->Layout();
  AddNode(node0);


  NodeGraphNode *node1 = new NodeGraphNode();
  node1->SetBounding(QRect(400, 100, 200, 100));
  node1->GetHeader()->SetName("Node1");
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


  painter.fillRect(0, 0, width(), height(), QColor(255, 255, 255));

  QTransform currentTransform = painter.transform();
  QTransform scaleTransform;
  scaleTransform.scale(1, 1);
  painter.setTransform(scaleTransform, true);


  for (NodeGraphNode *node : m_nodes)
  {
    node->paint(&painter);
  }

  painter.setTransform(currentTransform);
}