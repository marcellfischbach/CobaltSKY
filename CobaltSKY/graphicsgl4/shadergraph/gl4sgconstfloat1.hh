#pragma once

#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>

#include <graphicsgl4/shadergraph/gl4sgconstfloat1.refl.hh>

CS_CLASS()
class CSGRAPHICSGL4_API csSGConstFloatGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGConstFloatGL4() : csSGNodeGL4() { }
  virtual ~csSGConstFloatGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
