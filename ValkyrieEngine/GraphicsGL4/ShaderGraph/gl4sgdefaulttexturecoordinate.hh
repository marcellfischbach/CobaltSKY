#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgdefaulttexturecoordinate.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkSGDefaultTextureCoordinateGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGDefaultTextureCoordinateGL4() : vkSGNodeGL4() { }
  virtual ~vkSGDefaultTextureCoordinateGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
