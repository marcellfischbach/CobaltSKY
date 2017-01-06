#include <GraphicsGL4/ShaderGraph/gl4sgconstfloat4.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <sstream>


void vkSGConstFloat4GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();

  std::ostringstream ss;
  ss << "vec4("
    << std::to_string(n->GetInput(0)->GetConst()) << ", "
    << std::to_string(n->GetInput(1)->GetConst()) << ", "
    << std::to_string(n->GetInput(2)->GetConst()) << ", "
    << std::to_string(n->GetInput(3)->GetConst())
    << ")";
  ctx.SetOutputValue(n->GetOutput(0), ss.str());
}
