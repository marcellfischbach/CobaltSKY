#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgsplitfloat2.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGSplitFloat2GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat2GL4();
  virtual ~vkSGSplitFloat2GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};