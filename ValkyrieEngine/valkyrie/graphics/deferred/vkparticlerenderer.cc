

#include <valkyrie/graphics/deferred/vkparticlerenderer.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/entity/vkrenderstate.hh>

vkParticleRenderer::vkParticleRenderer()
{

}

vkParticleRenderer::~vkParticleRenderer()
{

}

void vkParticleRenderer::Render(iGraphics *renderer, vkCollection<vkRenderState*> &particles)
{
  renderer->SetBlendEnabled(true);
  renderer->SetDepthMask(false);
  for (vkSize i = 0; i<particles.length; ++i)
  {
    vkRenderState *renderState = particles[i];
    if (!renderState)
    {
      continue;
    }

    renderState->Render(renderer, eRP_ForwardUnlit);
  }
}

