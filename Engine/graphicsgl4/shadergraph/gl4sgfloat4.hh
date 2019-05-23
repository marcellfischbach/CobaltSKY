#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgfloat4.refl.hh>


namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGFloat4GL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGFloat4GL4() : cs::SGNodeGL4() { }
  virtual ~SGFloat4GL4() { }

protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}
