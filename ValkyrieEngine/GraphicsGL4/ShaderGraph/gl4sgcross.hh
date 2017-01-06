#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgcross.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkSGCrossGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGCrossGL4() : vkSGNodeGL4() { }
  virtual ~vkSGCrossGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
