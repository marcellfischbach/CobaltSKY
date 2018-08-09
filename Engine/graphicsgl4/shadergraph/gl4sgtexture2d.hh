#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>

#include <graphicsgl4/shadergraph/gl4sgtexture2d.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGTexture2DGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGTexture2DGL4() : csSGNodeGL4() { }
  virtual ~csSGTexture2DGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
