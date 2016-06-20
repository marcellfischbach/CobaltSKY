

#include <GraphicsGL4/Deferred/ParticleRendererGL4.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Entity/RenderState.hh>

vkParticleRendererGL4::vkParticleRendererGL4()
{

}

vkParticleRendererGL4::~vkParticleRendererGL4()
{

}

void vkParticleRendererGL4::Render(IGraphics *renderer, vkCollection<vkRenderState*> &particles)
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

    renderState->Render(renderer, eRP_Forward);
  }
}

