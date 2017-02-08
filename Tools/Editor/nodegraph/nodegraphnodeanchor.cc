
#include <nodegraph/nodegraphnodeanchor.hh>

NodeGraphNodeAnchor::NodeGraphNodeAnchor()
{


}

bool NodeGraphNodeAnchor::Test(const QPointF &point) const
{
  return m_bounds.contains(point);
}

