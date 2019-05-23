
#include <graphicsgl4/shadergraph/gl4sgconstfloat1.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>

void csSGConstFloatGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  cs::SGNode *n = GetNode();

  ctx.SetOutputValue(n->GetOutput(0), std::to_string(n->GetInput(0)->GetConst()));
}
