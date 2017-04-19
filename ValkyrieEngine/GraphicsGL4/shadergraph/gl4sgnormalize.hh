#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgnormalize.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGNormalizeGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGNormalizeGL4() : vkSGNodeGL4() { }
  virtual ~vkSGNormalizeGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
