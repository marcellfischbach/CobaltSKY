#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgdiv.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkSGDivGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGDivGL4() : vkSGNodeGL4() { }
  virtual ~vkSGDivGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
