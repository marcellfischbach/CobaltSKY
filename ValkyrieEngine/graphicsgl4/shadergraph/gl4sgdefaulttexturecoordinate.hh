#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgdefaulttexturecoordinate.refl.hh>

CS_CLASS()
class CSGRAPHICSGL4_API csSGDefaultTextureCoordinateGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGDefaultTextureCoordinateGL4() : csSGNodeGL4() { }
  virtual ~csSGDefaultTextureCoordinateGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
