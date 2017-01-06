#pragma once
#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>

#include <GraphicsGL4/ShaderGraph/gl4sgtexture2d.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGTexture2DGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGTexture2DGL4() : vkSGNodeGL4() { }
  virtual ~vkSGTexture2DGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
