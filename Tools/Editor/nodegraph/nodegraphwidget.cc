

#include <nodegraph/nodegraphwidget.hh>
#include <nodegraph/nodegraphnode.hh>

#include <QPaintEvent>
#include <QPainter>

NodeGraphWidget::NodeGraphWidget(QWidget *parent)
  : QWidget(parent)
{

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

  painter.fillRect(0, 0, width(), height(), QColor(64, 64, 64));
}