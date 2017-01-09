#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/graphics/vkpostprocess.hh>

#include <valkyrie/graphics/vkgenericshaderpostprocess.refl.hh>

VK_CLASS()
class VKE_API vkGenericShaderPostProcess : public VK_SUPER(vkPostProcess)
{
  VK_CLASS_GEN;
public:
  vkGenericShaderPostProcess();
  virtual ~vkGenericShaderPostProcess();

  virtual bool Render(iGraphics *graphics);

  void SetShader(iShader *shader);


};
