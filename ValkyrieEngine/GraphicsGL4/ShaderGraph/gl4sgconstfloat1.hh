#pragma once

#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>

#include <GraphicsGL4/ShaderGraph/gl4sgconstfloat1.refl.hh>

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
