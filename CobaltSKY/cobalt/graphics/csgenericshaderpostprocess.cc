
#include <cobalt/graphics/csgenericshaderpostprocess.hh>





csGenericShaderPostProcess::csGenericShaderPostProcess()
  : csPostProcess()
{

}

csGenericShaderPostProcess::~csGenericShaderPostProcess()
{

}


void csGenericShaderPostProcess::SetShader(iShader *shader)
{
  csPostProcess::SetShader(shader);
}


bool csGenericShaderPostProcess::Render(iGraphics *graphics)
{
  if (!csPostProcess::Render(graphics))
  {
    return false;
  }

  graphics->RenderFullScreenFrame();
  return true;
}



