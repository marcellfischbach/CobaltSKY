#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/graphics/scene/csspatialnode.hh>
#include <cobalt/graphics/scene/cslightnode.refl.hh>


namespace cs
{
struct iGraphics;
class Light;

CS_CLASS()
class CSE_API LightNode : public CS_SUPER(cs::SpatialNode)
{
  CS_CLASS_GEN;
public:
  LightNode();
  virtual ~LightNode();

  void SetLight(cs::Light * light);
  cs::Light* GetLight();
  const cs::Light* GetLight() const;

protected:
  virtual void PrivScan(const cs::Clipper * clipper, cs::iGraphics * renderer, cs::iScanCallback * callback);

  virtual void UpdateBoundingBox(cs::BoundingBox & bbox);

private:
  cs::Light* m_light;

};

}

CS_FORCEINLINE cs::Light *cs::LightNode::GetLight()
{
  return m_light;
}

CS_FORCEINLINE const cs::Light *cs::LightNode::GetLight() const
{
  return m_light;
}
