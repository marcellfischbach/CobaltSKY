#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgsplitfloat3.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGSplitFloat3GL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGSplitFloat3GL4();
  virtual ~csSGSplitFloat3GL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
