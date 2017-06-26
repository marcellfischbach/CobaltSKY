#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgfloat4.refl.hh>



CS_CLASS()
class CSGRAPHICSGL4_API csSGFloat4GL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGFloat4GL4() : csSGNodeGL4() { }
  virtual ~csSGFloat4GL4() { }

protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
