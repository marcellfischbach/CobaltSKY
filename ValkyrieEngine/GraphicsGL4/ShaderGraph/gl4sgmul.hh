#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgmul.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGMulGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGMulGL4() : vkSGNodeGL4() { }
  virtual ~vkSGMulGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
