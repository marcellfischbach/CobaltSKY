#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgcross.refl.hh>

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
