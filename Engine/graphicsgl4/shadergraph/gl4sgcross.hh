#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgcross.refl.hh>


namespace cs
{


CS_CLASS()
class CSGRAPHICSGL4_API SGCrossGL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGCrossGL4() : cs::SGNodeGL4() { }
  virtual ~SGCrossGL4() { }

protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}
