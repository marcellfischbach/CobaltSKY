

#include <Valkyrie/graphics/scene/vklightnode.hh>
#include <Valkyrie/graphics/vkdirectionallight.hh>
#include <Valkyrie/graphics/vkpointlight.hh>
#include <Valkyrie/graphics/scene/iscancallback.hh>

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