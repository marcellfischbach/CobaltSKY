#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgadd.refl.hh>



VK_CLASS()
class VKGRAPHICSGL4_API vkSGAddGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGAddGL4() : vkSGNodeGL4() { }
  virtual ~vkSGAddGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
