
#include <graphicsgl4/shadergraph/gl4sgconstfloat1.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>

void cs::SGConstFloatGL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  cs::SGNode *n = GetNode();

  ctx.SetOutputValue(n->GetOutput(0), std::to_string(n->GetInput(0)->GetConst()));
}
