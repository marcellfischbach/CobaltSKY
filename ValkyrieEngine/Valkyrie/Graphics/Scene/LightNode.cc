

#include <Valkyrie/Graphics/Scene/LightNode.hh>
#include <Valkyrie/Graphics/Light.hh>
#include <Valkyrie/Graphics/Scene/Scan.hh>

vkLightNode::vkLightNode()
  : vkSpatialNode()
  , m_light(0)
{

}

vkLightNode::~vkLightNode()
{
  VK_RELEASE(m_light);
}

void vkLightNode::SetLight(vkLight *light)
{
  VK_SET(m_light, light);
}



void vkLightNode::PrivScan(const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback)
{
  callback->ScanLightNode(this);
}