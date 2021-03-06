#include <graphicsgl4/shadergraph/gl4sgconstfloat2.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>

void cs::SGConstFloat2GL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  cs::SGNode *n = GetNode();

  std::ostringstream ss;
  ss << "vec2("
    << std::to_string(n->GetInput(0)->GetConst()) << ", "
    << std::to_string(n->GetInput(1)->GetConst())
    << ")";
  ctx.SetOutputValue(n->GetOutput(0), ss.str());
}
