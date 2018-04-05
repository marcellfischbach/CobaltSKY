

#include <graphicsgl4/deferred/gl4particlerenderer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/entity/csrenderstate.hh>

csParticleRendererGL4::csParticleRendererGL4()
{

}

csParticleRendererGL4::~csParticleRendererGL4()
{

}

void csParticleRendererGL4::Render(iGraphics *renderer, csCollection<csRenderState*> &particles)
{
  renderer->SetBlendEnabled(true);
  renderer->SetDepthMask(false);
  for (csSize i = 0; i<particles.length; ++i)
  {
    csRenderState *renderState = particles[i];
    if (!renderState)
    {
      continue;
    }

    renderState->Render(renderer, eRP_ForwardUnlit);
  }
}

