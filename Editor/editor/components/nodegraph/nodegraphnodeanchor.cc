
#include <editor/nodegraph/nodegraphnodeanchor.hh>
#include <editor/nodegraph/nodegraphnode.hh>

NodeGraphNodeAnchor::NodeGraphNodeAnchor(NodeGraphNode *node, NodeGraphNodeProperty *prop)
  : m_node(node)
  , m_connected(false)
  , m_prop(prop)
{


}

bool NodeGraphNodeAnchor::Test(const QPointF &point) const
{
  return m_bounds.contains(point);
}


QPointF NodeGraphNodeAnchor::GetCenter() const
{
  return m_bounds.center();
}

QPointF NodeGraphNodeAnchor::GetAbsCenter() const
{
  return m_node->GetBounding().topLeft() + m_bounds.center();
}
