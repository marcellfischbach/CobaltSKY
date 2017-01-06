#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgfloat4.refl.hh>



VK_CLASS()
class VKGRAPHICSGL4_API vkSGFloat4GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGFloat4GL4() : vkSGNodeGL4() { }
  virtual ~vkSGFloat4GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
