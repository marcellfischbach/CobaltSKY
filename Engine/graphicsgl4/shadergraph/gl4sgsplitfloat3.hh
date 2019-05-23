#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgsplitfloat3.refl.hh>

namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGSplitFloat3GL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGSplitFloat3GL4();
  virtual ~SGSplitFloat3GL4() { }

protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}
