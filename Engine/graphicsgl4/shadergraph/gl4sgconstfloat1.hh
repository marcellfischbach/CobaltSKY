#pragma once

#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>

#include <graphicsgl4/shadergraph/gl4sgconstfloat1.refl.hh>

namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGConstFloatGL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGConstFloatGL4() : cs::SGNodeGL4() { }
  virtual ~SGConstFloatGL4() { }

protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}
