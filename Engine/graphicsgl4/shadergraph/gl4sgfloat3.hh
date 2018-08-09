#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgfloat3.refl.hh>

CS_CLASS()
class CSGRAPHICSGL4_API csSGFloat3GL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGFloat3GL4() : csSGNodeGL4() { }
  virtual ~csSGFloat3GL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
