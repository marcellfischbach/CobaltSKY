#pragma once

#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.refl.hh>

namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGVarGL4 : public CS_SUPER(cs::SGNodeGL4)
{
  CS_CLASS_GEN;
public:
  SGVarGL4() : cs::SGNodeGL4() { }
  SGVarGL4(const std::string & typeName);
  virtual ~SGVarGL4() { }
private:
  std::string m_typeName;
protected:
  void PrivEvaluate(cs::ShaderGraphCtx & ctx);
};

}
