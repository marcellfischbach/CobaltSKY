
#include <valkyrie/graphics/vkgenericshaderpostprocess.hh>





vkGenericShaderPostProcess::vkGenericShaderPostProcess()
  : vkPostProcess()
{

}

vkGenericShaderPostProcess::~vkGenericShaderPostProcess()
{

}


void vkGenericShaderPostProcess::SetShader(IShader *shader)
{
  vkPostProcess::SetShader(shader);
}


bool vkGenericShaderPostProcess::Render(IGraphics *graphics)
{
  if (!vkPostProcess::Render(graphics))
  {
    return false;
  }

  graphics->RenderFullScreenFrame();
  return true;
}



