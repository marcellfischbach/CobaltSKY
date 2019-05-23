

#include <graphicsgl4/deferred/gl4particlerenderer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/entity/csrenderstate.hh>

csParticleRendererGL4::csParticleRendererGL4()
{

}

csParticleRendererGL4::~csParticleRendererGL4()
{

}

void csParticleRendererGL4::Render(cs::iGraphics *renderer, cs::Collection<cs::RenderState*> &particles)
{
  renderer->SetBlendEnabled(true);
  renderer->SetDepthMask(false);
  for (csSize i = 0; i<particles.length; ++i)
  {
    cs::RenderState *renderState = particles[i];
    if (!renderState)
    {
      continue;
    }

    renderState->Render(renderer, cs::eRP_ForwardUnlit);
  }
}

