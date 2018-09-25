#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgsplitfloat2.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGSplitFloat2GL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGSplitFloat2GL4();
  virtual ~csSGSplitFloat2GL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};