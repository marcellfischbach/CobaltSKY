#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgnormalize.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGNormalizeGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGNormalizeGL4() : csSGNodeGL4() { }
  virtual ~csSGNormalizeGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
