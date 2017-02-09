
#include <nodegraph/nodegraphnodeanchor.hh>
#include <nodegraph/nodegraphnode.hh>

NodeGraphNodeAnchor::NodeGraphNodeAnchor(NodeGraphNode *node)
  : m_node(node)
  , m_connected(false)
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
