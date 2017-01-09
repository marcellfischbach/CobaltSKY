
#include <valkyrie/graphics/vkgenericshaderpostprocess.hh>





vkGenericShaderPostProcess::vkGenericShaderPostProcess()
  : vkPostProcess()
{

}

vkGenericShaderPostProcess::~vkGenericShaderPostProcess()
{

}


void vkGenericShaderPostProcess::SetShader(iShader *shader)
{
  vkPostProcess::SetShader(shader);
}


bool vkGenericShaderPostProcess::Render(iGraphics *graphics)
{
  if (!vkPostProcess::Render(graphics))
  {
    return false;
  }

  graphics->RenderFullScreenFrame();
  return true;
}



