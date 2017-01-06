#include <GraphicsGL4/ShaderGraph/gl4sgdefaulttexturecoordinate.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>


void vkSGDefaultTextureCoordinateGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *coord = GetNode();

  ctx.SetOutputValue(coord->GetOutput(0), ctx.GetDefaultTextureCoordinate());

}
