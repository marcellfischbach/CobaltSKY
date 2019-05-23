#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgneg.refl.hh>

namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGNegGL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGNegGL4() : cs::SGNodeGL4() { }
  virtual ~SGNegGL4() { }

protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}
