#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sglerp.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGLerpGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGLerpGL4() : vkSGNodeGL4() { }
  virtual ~vkSGLerpGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
