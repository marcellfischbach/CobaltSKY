#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sglerp.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGLerpGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGLerpGL4() : vkSGNodeGL4() { }
  virtual ~vkSGLerpGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
