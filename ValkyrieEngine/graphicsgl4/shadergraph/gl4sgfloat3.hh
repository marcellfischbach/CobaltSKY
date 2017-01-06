#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgfloat3.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkSGFloat3GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGFloat3GL4() : vkSGNodeGL4() { }
  virtual ~vkSGFloat3GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
