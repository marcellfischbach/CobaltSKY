#pragma once

#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.refl.hh>

CS_CLASS()
class CSGRAPHICSGL4_API csSGVarGL4 : public CS_SUPER(csSGNodeGL4)
{
  CS_CLASS_GEN;
public:
  csSGVarGL4() : csSGNodeGL4() { }
  csSGVarGL4(const csString &typeName);
  virtual ~csSGVarGL4() { }
private:
  csString m_typeName;
protected:
  void PrivEvaluate(csShaderGraphCtx &ctx);
};
