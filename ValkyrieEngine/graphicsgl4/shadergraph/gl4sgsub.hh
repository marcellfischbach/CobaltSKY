#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgsub.refl.hh>

CS_CLASS()
class CSGRAPHICSGL4_API csSGSubGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGSubGL4() : csSGNodeGL4() { }
  virtual ~csSGSubGL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
