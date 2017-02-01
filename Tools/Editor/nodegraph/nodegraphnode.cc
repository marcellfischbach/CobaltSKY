

#include <nodegraph/nodegraphnode.hh>

NodeGraphNode::NodeGraphNode()
{

}

NodeGraphNode::~NodeGraphNode()
{

}

void NodeGraphNode::paint(QPainter *painter)
{
  QTransform currentTransform = painter->transform();




  painter->setTransform(currentTransform, false);
}