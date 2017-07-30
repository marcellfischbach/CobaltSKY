#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/csenums.hh>
#include <cobalt/entity/csrenderstate.refl.hh>

struct iGraphics;
/**
* \ingroup entity
*/

CS_INTERFACE()
class CSE_API csRenderState : public CS_SUPER(csSpatialState)
{
  CS_CLASS_GEN;
public:
  csRenderState();
  virtual ~csRenderState();

  void SetFadeOut(float start, float end);

  void SetRenderQueue (csRenderQueue queue);
  csRenderQueue GetRenderQueue () const;

  void SetShadingMode (csShadingMode shadingMode);
  csShadingMode GetShadingMode() const;

  virtual unsigned GetNumberOfRenderCalls() const { return 0;  }
  virtual unsigned GetNumberOfTotalTrigons() const { return 0; }

  virtual void Render(iGraphics *graphics, csRenderPass pass) const;

private:
  float m_renderStartFade;
  float m_renderEndFade;
  csRenderQueue m_queue;
  csShadingMode m_shadingMode;
};

