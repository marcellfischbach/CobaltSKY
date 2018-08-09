#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgcross.refl.hh>

CS_CLASS()
class CSGRAPHICSGL4_API csSGCrossGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGCrossGL4() : csSGNodeGL4() { }
  virtual ~csSGCrossGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
