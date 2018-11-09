#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgmul.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGMulGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGMulGL4() : csSGNodeGL4() { }
  virtual ~csSGMulGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
