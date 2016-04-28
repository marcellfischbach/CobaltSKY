#pragma once

#include <ShaderGraph/Node.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Enums.hh>

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
  SGNode(const vkClass *nodeClass, const vkSGNode *originNode);
  void AddConnection(graph::NodeConnection* connection);

  void RemoveConnection(graph::NodeConnection* connection);

  void RemoveAllConnections();

  const vkClass* GetClass() const
  {
    return m_nodeClass;
  }

  bool IsResources() const
  {
    return m_resource;
  }
 
  vkShaderParameterType GetResourceType() const
  {
    return m_resourceType;
  }

  float *GetDefaultFloat()
  {
    return m_defaultFloat;
  }
  int *GetDefaultInt()
  {
    return m_defaultInt;
  }
  vkResourceLocator &GetDefaultTexture()
  {
    return m_defaultTexture;
  }

private:
  const vkClass *m_nodeClass;

  bool m_resource;

  vkShaderParameterType m_resourceType;
  float m_defaultFloat[16];
  int m_defaultInt[16];
  vkResourceLocator m_defaultTexture;

};

}