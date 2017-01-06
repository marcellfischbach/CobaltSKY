#pragma once

#include <valkyrie/core/vkcollection.hh>

struct IGraphics;
class vkRenderState;

class vkParticleRenderer
{
public:
  vkParticleRenderer();
  virtual ~vkParticleRenderer();

  void Render(IGraphics *renderer, vkCollection<vkRenderState*> &particles);
};

