#pragma once

#include <Graph/Node.hh>

namespace shadergraph
{

class Node : public graph::GraphNode
{
public:
  enum Type
  {
    eT_Node,
    eT_ShaderGraph,
  };
  Type GetType() const;
protected:
  Node(Type type);

private:
  Type m_type;
};

}
