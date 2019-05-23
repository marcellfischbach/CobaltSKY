#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgconstfloat3.refl.hh>


namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGConstFloat3GL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGConstFloat3GL4() : cs::SGNodeGL4() { }
  virtual ~SGConstFloat3GL4() { }

protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}
