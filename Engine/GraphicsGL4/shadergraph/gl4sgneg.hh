#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgneg.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGNegGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGNegGL4() : csSGNodeGL4() { }
  virtual ~csSGNegGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
