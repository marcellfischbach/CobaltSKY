#pragma once

#include <ShaderGraph/Node.hh>
#include <Valkyrie/Core/Object.hh>
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
  SGNode(const vkClass *nodeClass);
  void AddConnection(graph::NodeConnection* connection);

  void RemoveConnection(graph::NodeConnection* connection);

  void RemoveAllConnections();

  const vkClass* GetClass() const
  {
    return m_nodeClass;
  }

private:
  const vkClass *m_nodeClass;
};

}