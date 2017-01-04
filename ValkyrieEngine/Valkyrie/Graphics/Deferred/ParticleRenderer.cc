

#include <Valkyrie/Graphics/Deferred/ParticleRenderer.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/entity/vkrenderstate.hh>

vkParticleRenderer::vkParticleRenderer()
{

}

vkParticleRenderer::~vkParticleRenderer()
{

}

void vkParticleRenderer::Render(IGraphics *renderer, vkCollection<vkRenderState*> &particles)
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

