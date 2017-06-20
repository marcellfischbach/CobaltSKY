#pragma once

#include <cobalt/core/cscollection.hh>

struct iGraphics;
class csRenderState;

class csParticleRenderer
{
public:
  csParticleRenderer();
  virtual ~csParticleRenderer();

  void Render(iGraphics *renderer, csCollection<csRenderState*> &particles);
};

