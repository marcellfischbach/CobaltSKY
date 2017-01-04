#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Entity/RenderState.refl.hh>

struct IGraphics;
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

  virtual void Render(IGraphics *graphics, vkRenderPass pass) const;

private:
  float m_renderStartFade;
  float m_renderEndFade;
  vkRenderQueue m_queue;
  vkShadingMode m_shadingMode;
};

