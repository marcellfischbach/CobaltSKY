#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgsplitfloat2.refl.hh>

namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGSplitFloat2GL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGSplitFloat2GL4();
  virtual ~SGSplitFloat2GL4() { }

protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}
