

#include <cobalt/graphics/scene/cslightnode.hh>
#include <cobalt/graphics/csdirectionallight.hh>
#include <cobalt/graphics/cspointlight.hh>
#include <cobalt/graphics/scene/iscancallback.hh>
#include <float.h>

cs::LightNode::LightNode()
  : cs::SpatialNode()
  , m_light(0)
{

}

cs::LightNode::~LightNode()
{
  CS_RELEASE(m_light);
}

void cs::LightNode::SetLight(cs::Light *light)
{
  CS_SET(m_light, light);
}



void cs::LightNode::PrivScan(const cs::Clipper *clipper, cs::iGraphics *renderer, cs::iScanCallback *callback)
{
  callback->ScanLightNode(this);
}

void cs::LightNode::UpdateBoundingBox(cs::BoundingBox &bbox)
{
  if (m_light)
  {
    switch (m_light->GetLightType())
    {
    case cs::eLT_PointLight:
      {
        cs::Vector3f pos = GetMatrix().GetTranslation(pos);
        float radius = ((cs::PointLight*)m_light)->GetRadius();
        bbox.Add(cs::Vector3f(pos.x - radius, pos.y - radius, pos.z - radius));
        bbox.Add(cs::Vector3f(pos.x + radius, pos.y + radius, pos.z + radius));
      }
      break;
    case cs::eLT_DirectionalLight:
      bbox.Add(cs::Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX));
      bbox.Add(cs::Vector3f(FLT_MAX, FLT_MAX, FLT_MAX));
      break;
    }
    bbox.Finish();
  }
}
