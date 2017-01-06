#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgdiv.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkSGDivGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGDivGL4() : vkSGNodeGL4() { }
  virtual ~vkSGDivGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
