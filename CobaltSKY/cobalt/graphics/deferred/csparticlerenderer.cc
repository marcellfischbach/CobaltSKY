

#include <cobalt/graphics/deferred/csparticlerenderer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/entity/csrenderstate.hh>

csParticleRenderer::csParticleRenderer()
{

}

csParticleRenderer::~csParticleRenderer()
{

}

void csParticleRenderer::Render(iGraphics *renderer, csCollection<csRenderState*> &particles)
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

