#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/graphics/scene/vkspatialnode.hh>
#include <valkyrie/graphics/scene/vklightnode.refl.hh>

struct IGraphics;
class vkLight;
VK_CLASS()
class VKE_API vkLightNode : public VK_SUPER(vkSpatialNode)
{
  VK_CLASS_GEN;
public:
  vkLightNode();
  virtual ~vkLightNode();

  void SetLight(vkLight *light);
  vkLight *GetLight();
  const vkLight *GetLight() const;

protected:
  virtual void PrivScan(const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback);

  virtual void UpdateBoundingBox(vkBoundingBox &bbox);

private:
  vkLight *m_light;

};


VK_FORCEINLINE vkLight *vkLightNode::GetLight()
{
  return m_light;
}

VK_FORCEINLINE const vkLight *vkLightNode::GetLight() const
{
  return m_light;
}
