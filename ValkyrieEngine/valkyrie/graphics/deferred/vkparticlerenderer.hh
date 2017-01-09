#pragma once

#include <valkyrie/core/vkcollection.hh>

struct iGraphics;
class vkRenderState;

class vkParticleRenderer
{
public:
  vkParticleRenderer();
  virtual ~vkParticleRenderer();

  void Render(iGraphics *renderer, vkCollection<vkRenderState*> &particles);
};

