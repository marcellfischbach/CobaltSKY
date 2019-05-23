#include <graphicsgl4/shadergraph/gl4sgdefaulttexturecoordinate.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>


void cs::SGDefaultTextureCoordinateGL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  cs::SGNode *coord = GetNode();

  ctx.SetOutputValue(coord->GetOutput(0), ctx.GetDefaultTextureCoordinate());

}
