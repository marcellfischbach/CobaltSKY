#pragma once

#include <ShaderGraph/Node.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Enums.hh>

class vkSGNode;
class vkSGResourceNode;
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

  void UpdateResource ();

  const vkSGNode *GetNode () const
  {
    return m_node;
  }

  vkSGNode *GetNode ()
  {
    return m_node;
  }


private:
  vkSGNode *m_node;
  vkSGResourceNode *m_res;

};

}
