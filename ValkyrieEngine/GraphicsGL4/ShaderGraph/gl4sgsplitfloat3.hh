#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgsplitfloat3.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGSplitFloat3GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat3GL4();
  virtual ~vkSGSplitFloat3GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
