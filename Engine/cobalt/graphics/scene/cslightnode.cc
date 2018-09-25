

#include <cobalt/graphics/scene/cslightnode.hh>
#include <cobalt/graphics/csdirectionallight.hh>
#include <cobalt/graphics/cspointlight.hh>
#include <cobalt/graphics/scene/iscancallback.hh>
#include <float.h>

csLightNode::csLightNode()
  : csSpatialNode()
  , m_light(0)
{

}

csLightNode::~csLightNode()
{
  CS_RELEASE(m_light);
}

void csLightNode::SetLight(csLight *light)
{
  CS_SET(m_light, light);
}



void csLightNode::PrivScan(const csClipper *clipper, iGraphics *renderer, iScanCallback *callback)
{
  callback->ScanLightNode(this);
}

void csLightNode::UpdateBoundingBox(csBoundingBox &bbox)
{
  if (m_light)
  {
    switch (m_light->GetLightType())
    {
    case eLT_PointLight:
      {
        csVector3f pos = GetMatrix().GetTranslation(pos);
        float radius = ((csPointLight*)m_light)->GetRadius();
        bbox.Add(csVector3f(pos.x - radius, pos.y - radius, pos.z - radius));
        bbox.Add(csVector3f(pos.x + radius, pos.y + radius, pos.z + radius));
      }
      break;
    case eLT_DirectionalLight:
      bbox.Add(csVector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX));
      bbox.Add(csVector3f(FLT_MAX, FLT_MAX, FLT_MAX));
      break;
    }
    bbox.Finish();
  }
}
