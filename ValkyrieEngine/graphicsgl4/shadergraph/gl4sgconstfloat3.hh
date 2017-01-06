#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgconstfloat3.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkSGConstFloat3GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat3GL4() : vkSGNodeGL4() { }
  virtual ~vkSGConstFloat3GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
