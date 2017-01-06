#pragma once

#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkSGVarGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarGL4() : vkSGNodeGL4() { }
  vkSGVarGL4(const vkString &typeName);
  virtual ~vkSGVarGL4() { }
private:
  vkString m_typeName;
protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
