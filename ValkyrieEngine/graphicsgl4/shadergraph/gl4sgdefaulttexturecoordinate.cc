#include <graphicsgl4/shadergraph/gl4sgdefaulttexturecoordinate.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>


void vkSGDefaultTextureCoordinateGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *coord = GetNode();

  ctx.SetOutputValue(coord->GetOutput(0), ctx.GetDefaultTextureCoordinate());

}
