#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sglerp.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGLerpGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGLerpGL4() : csSGNodeGL4() { }
  virtual ~csSGLerpGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
