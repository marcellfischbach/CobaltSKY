#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/entity/vkspatialstate.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/entity/vkrenderstate.refl.hh>

struct iGraphics;
/**
* \ingroup entity
*/

VK_INTERFACE()
class VKE_API vkRenderState : public VK_SUPER(vkSpatialState)
{
  VK_CLASS_GEN;
public:
  vkRenderState();
  virtual ~vkRenderState();

  void SetFadeOut(float start, float end);

  void SetRenderQueue (vkRenderQueue queue);
  vkRenderQueue GetRenderQueue () const;

  void SetShadingMode (vkShadingMode shadingMode);
  vkShadingMode GetShadingMode() const;

  virtual void Render(iGraphics *graphics, vkRenderPass pass) const;

private:
  float m_renderStartFade;
  float m_renderEndFade;
  vkRenderQueue m_queue;
  vkShadingMode m_shadingMode;
};

