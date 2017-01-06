#pragma once

#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>

#include <graphicsgl4/shadergraph/gl4sgconstfloat1.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkSGConstFloatGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloatGL4() : vkSGNodeGL4() { }
  virtual ~vkSGConstFloatGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
