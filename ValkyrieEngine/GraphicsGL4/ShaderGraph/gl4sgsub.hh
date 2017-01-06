#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgsub.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkSGSubGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGSubGL4() : vkSGNodeGL4() { }
  virtual ~vkSGSubGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
