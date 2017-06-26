#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgfloat2.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGFloat2GL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGFloat2GL4() : csSGNodeGL4() { }
  virtual ~csSGFloat2GL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
