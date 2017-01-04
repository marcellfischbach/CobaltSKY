#pragma once

#include <Valkyrie/Core/vkcollection.hh>

struct IGraphics;
class vkRenderState;

class vkParticleRenderer
{
public:
  vkParticleRenderer();
  virtual ~vkParticleRenderer();

  void Render(IGraphics *renderer, vkCollection<vkRenderState*> &particles);
};

