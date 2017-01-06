#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgconstfloat4.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGConstFloat4GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat4GL4() : vkSGNodeGL4() { }
  virtual ~vkSGConstFloat4GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
