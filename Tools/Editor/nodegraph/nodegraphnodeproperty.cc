

#include <nodegraph/nodegraphnodeproperty.hh>

NodeGraphNodeProperty::NodeGraphNodeProperty(NodeGraphNode *node)
  : m_minSizeDirty(true)
  , m_node(node)
{

}

NodeGraphNodeProperty::~NodeGraphNodeProperty()
{

}
