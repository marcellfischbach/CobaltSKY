#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgdot.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGDotGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGDotGL4() : vkSGNodeGL4() { }
  virtual ~vkSGDotGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
