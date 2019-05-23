#include <graphicsgl4/shadergraph/gl4sgconstfloat4.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>


void csSGConstFloat4GL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  cs::SGNode *n = GetNode();

  std::ostringstream ss;
  ss << "vec4("
    << std::to_string(n->GetInput(0)->GetConst()) << ", "
    << std::to_string(n->GetInput(1)->GetConst()) << ", "
    << std::to_string(n->GetInput(2)->GetConst()) << ", "
    << std::to_string(n->GetInput(3)->GetConst())
    << ")";
  ctx.SetOutputValue(n->GetOutput(0), ss.str());
}
