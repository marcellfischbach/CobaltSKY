#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgoneminus.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGOneMinusGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGOneMinusGL4() : csSGNodeGL4() { }
  virtual ~csSGOneMinusGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
