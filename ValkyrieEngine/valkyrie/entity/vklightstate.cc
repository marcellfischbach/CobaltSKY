
#include <valkyrie/entity/vklightstate.hh>
#include <valkyrie/entity/ientityscan.hh>
#include <valkyrie/graphics/vklight.hh>



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


void vkLightState::PrivScan(vkClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const vkScanConfig &config)
{
  if (m_light)
  {
    entityScan->ScanLightState(this);
  }
}

void vkLightState::FillBoundingBox(vkBoundingBox &bbox)
{
  if (m_light)
  {
    switch (m_light->GetLightType())
    {
    case eLT_DirectionalLight:
      bbox.Add(vkVector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX));
      bbox.Add(vkVector3f(FLT_MAX, FLT_MAX, FLT_MAX));
      break;
    }
  }
}