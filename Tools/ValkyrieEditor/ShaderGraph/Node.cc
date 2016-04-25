
#include <ShaderGraph/Node.hh>


shadergraph::Node::Node(shadergraph::Node::Type type) 
  : graph::Node()
  , m_type(type) 
{ 
}

shadergraph::Node::Type shadergraph::Node::GetType() const
{
  return m_type;
}
