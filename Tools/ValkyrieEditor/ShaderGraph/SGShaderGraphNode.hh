#pragma once


#include <ShaderGraph/Node.hh>

class vkSGShaderGraph;

namespace graph
{
class Connection;
}
namespace shadergraph
{

class SGShaderGraphNode : public Node
{
public:
  SGShaderGraphNode(vkSGShaderGraph *graph);

  void AddConnection(graph::NodeConnection* connection);
  void RemoveConnection(graph::NodeConnection* connection);


public:
  vkSGShaderGraph *GetShaderGraph()
  {
    return m_graph;
  }

  const vkSGShaderGraph *GetShaderGraph() const
  {
    return m_graph;
  }

private:
  vkSGShaderGraph *m_graph;
};

}