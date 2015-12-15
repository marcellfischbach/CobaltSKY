
#include <Valkyrie/Entity/LightState.hh>
#include <Valkyrie/Entity/Scan.hh>
#include <Valkyrie/Graphics/Light.hh>



vkLightState::vkLightState()
  : vkSpatialState()
  , m_light(0)
{

}

vkLightState::~vkLightState()
{

}


void vkLightState::SetLight(vkLight *light)
{
  VK_SET(m_light, light);
}


void vkLightState::PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config)
{
  if (m_light)
  {
    entityScan->ScanLight(m_light);
  }
}