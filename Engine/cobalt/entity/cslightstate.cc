
#include <cobalt/entity/cslightstate.hh>
#include <cobalt/entity/ientityscan.hh>
#include <cobalt/graphics/cslight.hh>



cs::LightState::LightState()
  : cs::SpatialState()
  , m_light(0)
{

}

cs::LightState::~LightState()
{

}


void cs::LightState::SetLight(cs::Light *light)
{
  CS_SET(m_light, light);
}


void cs::LightState::PrivScan(cs::Clipper *clipper, cs::iGraphics *graphics, cs::iEntityScan *entityScan, const cs::ScanConfig &config)
{
  if (m_light)
  {
    entityScan->ScanLightState(this);
  }
}

void cs::LightState::FillBoundingBox(cs::BoundingBox &bbox)
{
  if (m_light)
  {
    switch (m_light->GetLightType())
    {
    case cs::eLT_DirectionalLight:
      bbox.Add(cs::Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX));
      bbox.Add(cs::Vector3f(FLT_MAX, FLT_MAX, FLT_MAX));
      break;
    }
  }
}