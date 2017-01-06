
#include <GraphicsGL4/ShaderGraph/gl4sgconstfloat1.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>

void vkSGConstFloatGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();

  ctx.SetOutputValue(n->GetOutput(0), std::to_string(n->GetInput(0)->GetConst()));
}
