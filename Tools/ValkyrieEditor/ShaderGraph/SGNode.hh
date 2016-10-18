#pragma once

#include <ShaderGraph/Node.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Enums.hh>
#include <QMap>

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

  QMap<QString, graph::AttribInputWidget*> m_inputs;
  QMap<QString, graph::AttribOutputWidget*> m_outputs;

};

}
