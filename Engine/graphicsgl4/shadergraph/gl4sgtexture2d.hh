#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>

#include <graphicsgl4/shadergraph/gl4sgtexture2d.refl.hh>

namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGTexture2DGL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGTexture2DGL4() : cs::SGNodeGL4() { }
  virtual ~SGTexture2DGL4() { }

protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}
