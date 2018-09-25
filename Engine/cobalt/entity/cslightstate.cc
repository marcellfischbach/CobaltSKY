
#include <cobalt/entity/cslightstate.hh>
#include <cobalt/entity/ientityscan.hh>
#include <cobalt/graphics/cslight.hh>



csLightState::csLightState()
  : csSpatialState()
  , m_light(0)
{

}

csLightState::~csLightState()
{

}


void csLightState::SetLight(csLight *light)
{
  CS_SET(m_light, light);
}


void csLightState::PrivScan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config)
{
  if (m_light)
  {
    entityScan->ScanLightState(this);
  }
}

void csLightState::FillBoundingBox(csBoundingBox &bbox)
{
  if (m_light)
  {
    switch (m_light->GetLightType())
    {
    case eLT_DirectionalLight:
      bbox.Add(csVector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX));
      bbox.Add(csVector3f(FLT_MAX, FLT_MAX, FLT_MAX));
      break;
    }
  }
}