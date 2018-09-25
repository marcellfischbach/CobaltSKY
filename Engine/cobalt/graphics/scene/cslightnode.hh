#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/graphics/scene/csspatialnode.hh>
#include <cobalt/graphics/scene/cslightnode.refl.hh>

struct iGraphics;
class csLight;
CS_CLASS()
class CSE_API csLightNode : public CS_SUPER(csSpatialNode)
{
  CS_CLASS_GEN;
public:
  csLightNode();
  virtual ~csLightNode();

  void SetLight(csLight *light);
  csLight *GetLight();
  const csLight *GetLight() const;

protected:
  virtual void PrivScan(const csClipper *clipper, iGraphics *renderer, iScanCallback *callback);

  virtual void UpdateBoundingBox(csBoundingBox &bbox);

private:
  csLight *m_light;

};


CS_FORCEINLINE csLight *csLightNode::GetLight()
{
  return m_light;
}

CS_FORCEINLINE const csLight *csLightNode::GetLight() const
{
  return m_light;
}
