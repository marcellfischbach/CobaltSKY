#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgdot.refl.hh>

namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGDotGL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGDotGL4() : cs::SGNodeGL4() { }
  virtual ~SGDotGL4() { }

protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}
