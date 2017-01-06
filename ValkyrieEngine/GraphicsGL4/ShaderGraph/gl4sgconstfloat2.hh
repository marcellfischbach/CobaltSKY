#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgconstfloat2.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkSGConstFloat2GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat2GL4() : vkSGNodeGL4() { }
  virtual ~vkSGConstFloat2GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
