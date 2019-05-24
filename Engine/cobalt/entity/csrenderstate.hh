#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/csenums.hh>
#include <cobalt/entity/csrenderstate.refl.hh>

/**
* \ingroup entity
*/

namespace cs
{

struct iGraphics;

CS_CLASS()
class CSE_API RenderState : public CS_SUPER(cs::SpatialState)
{
  CS_CLASS_GEN;
public:
  RenderState();
  virtual ~RenderState();

  void SetFadeOut(float start, float end);

  void SetRenderQueue(cs::eRenderQueue queue);
  cs::eRenderQueue GetRenderQueue() const;

  void SetShadingMode(cs::eShadingMode shadingMode);
  cs::eShadingMode GetShadingMode() const;

  virtual size_t GetNumberOfRenderCalls() const { return 0; }
  virtual size_t GetNumberOfTotalTrigons() const { return 0; }

  virtual void Render(cs::iGraphics * graphics, cs::eRenderPass pass) const;

private:
  float m_renderStartFade;
  float m_renderEndFade;
  cs::eRenderQueue m_queue;
  cs::eShadingMode m_shadingMode;
};

}