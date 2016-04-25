#pragma once

#include <ShaderGraph/Node.hh>

class vkSGNode;
namespace graph
{
class Connection;
}
namespace shadergraph
{

class SGNode : public Node
{
public:
  SGNode(vkSGNode *node);
  void AddConnection(graph::NodeConnection* connection);

  void RemoveConnection(graph::NodeConnection* connection);

  void RemoveAllConnections();

  vkSGNode* GetNode()
  {
    return m_node;
  }
  const vkSGNode* GetNode() const
  {
    return m_node;
  }

private:
  vkSGNode *m_node;
};

}