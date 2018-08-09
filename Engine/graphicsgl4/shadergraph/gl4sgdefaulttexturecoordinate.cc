#include <graphicsgl4/shadergraph/gl4sgdefaulttexturecoordinate.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>


void csSGDefaultTextureCoordinateGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  csSGNode *coord = GetNode();

  ctx.SetOutputValue(coord->GetOutput(0), ctx.GetDefaultTextureCoordinate());

}
