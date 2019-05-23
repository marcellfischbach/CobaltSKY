
#include <cobalt/graphics/csgenericshaderpostprocess.hh>





cs::GenericShaderPostProcess::GenericShaderPostProcess()
  : cs::PostProcess()
{

}

cs::GenericShaderPostProcess::~GenericShaderPostProcess()
{

}


void cs::GenericShaderPostProcess::SetShader(cs::iShader *shader)
{
  cs::PostProcess::SetShader(shader);
}


bool cs::GenericShaderPostProcess::Render(cs::iGraphics *graphics)
{
  if (!cs::PostProcess::Render(graphics))
  {
    return false;
  }

  graphics->RenderFullScreenFrame();
  return true;
}



