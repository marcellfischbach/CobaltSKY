#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgadd.refl.hh>



CS_CLASS()
class CSGRAPHICSGL4_API csSGAddGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGAddGL4() : csSGNodeGL4() { }
  virtual ~csSGAddGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
