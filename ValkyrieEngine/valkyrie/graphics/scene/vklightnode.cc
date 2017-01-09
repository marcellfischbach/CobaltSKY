

#include <valkyrie/graphics/scene/vklightnode.hh>
#include <valkyrie/graphics/vkdirectionallight.hh>
#include <valkyrie/graphics/vkpointlight.hh>
#include <valkyrie/graphics/scene/iscancallback.hh>

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



void vkLightNode::PrivScan(const vkClipper *clipper, iGraphics *renderer, iScanCallback *callback)
{
  callback->ScanLightNode(this);
}

void vkLightNode::UpdateBoundingBox(vkBoundingBox &bbox)
{
  if (m_light)
  {
    switch (m_light->GetLightType())
    {
    case eLT_PointLight:
      {
        vkVector3f pos = GetMatrix().GetTranslation(pos);
        float radius = ((vkPointLight*)m_light)->GetRadius();
        bbox.Add(vkVector3f(pos.x - radius, pos.y - radius, pos.z - radius));
        bbox.Add(vkVector3f(pos.x + radius, pos.y + radius, pos.z + radius));
      }
      break;
    case eLT_DirectionalLight:
      bbox.Add(vkVector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX));
      bbox.Add(vkVector3f(FLT_MAX, FLT_MAX, FLT_MAX));
      break;
    }
    bbox.Finish();
  }
}