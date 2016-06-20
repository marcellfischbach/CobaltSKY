#pragma once

#include <Valkyrie/Core/Collection.hh>

struct IGraphics;
class vkRenderState;

class vkParticleRendererGL4
{
public:
  vkParticleRendererGL4();
  virtual ~vkParticleRendererGL4();

  void Render(IGraphics *renderer, vkCollection<vkRenderState*> &particles);
};

