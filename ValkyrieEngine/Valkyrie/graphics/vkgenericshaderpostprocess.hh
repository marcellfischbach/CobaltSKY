#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/graphics/vkpostprocess.hh>

#include <Valkyrie/graphics/vkgenericshaderpostprocess.refl.hh>

VK_CLASS()
class VKE_API vkGenericShaderPostProcess : public VK_SUPER(vkPostProcess)
{
  VK_CLASS_GEN;
public:
  vkGenericShaderPostProcess();
  virtual ~vkGenericShaderPostProcess();

  virtual bool Render(IGraphics *graphics);

  void SetShader(IShader *shader);


};
