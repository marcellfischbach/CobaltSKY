#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/cscollection.hh>


namespace cs
{
struct iGraphics;
class RenderState;

class ParticleRendererGL4
{
public:
  ParticleRendererGL4();
  virtual ~ParticleRendererGL4();

  void Render(cs::iGraphics *renderer, cs::Collection<cs::RenderState*> &particles);
};


}
