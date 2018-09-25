#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgdot.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGDotGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGDotGL4() : csSGNodeGL4() { }
  virtual ~csSGDotGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
