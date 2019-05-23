#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/cscollection.hh>


namespace cs
{
struct iGraphics;
class RenderState;
}

class csParticleRendererGL4
{
public:
  csParticleRendererGL4();
  virtual ~csParticleRendererGL4();

  void Render(cs::iGraphics *renderer, cs::Collection<cs::RenderState*> &particles);
};

