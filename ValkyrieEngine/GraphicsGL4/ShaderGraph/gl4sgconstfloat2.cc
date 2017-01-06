#include <GraphicsGL4/ShaderGraph/gl4sgconstfloat2.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <sstream>

void vkSGConstFloat2GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();

  std::ostringstream ss;
  ss << "vec2("
    << std::to_string(n->GetInput(0)->GetConst()) << ", "
    << std::to_string(n->GetInput(1)->GetConst())
    << ")";
  ctx.SetOutputValue(n->GetOutput(0), ss.str());
}
