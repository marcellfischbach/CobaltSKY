#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/cscollection.hh>

struct iGraphics;
class csRenderState;

class csParticleRendererGL4
{
public:
  csParticleRendererGL4();
  virtual ~csParticleRendererGL4();

  void Render(iGraphics *renderer, csCollection<csRenderState*> &particles);
};

