#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgdiv.refl.hh>

CS_CLASS()
class CSGRAPHICSGL4_API csSGDivGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGDivGL4() : csSGNodeGL4() { }
  virtual ~csSGDivGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
