#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgnormalize.refl.hh>

namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGNormalizeGL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGNormalizeGL4() : cs::SGNodeGL4() { }
  virtual ~SGNormalizeGL4() { }

protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}

