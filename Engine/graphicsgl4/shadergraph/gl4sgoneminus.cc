#include <graphicsgl4/shadergraph/gl4sgoneminus.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void cs::SGOneMinusGL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  cs::SGNode *mul = GetNode();
  ss << "(1 - " << ctx.GetFullInputValue(mul->GetInput(0)).c_str() << ")";

  cs::eSGDataType dt = mul->GetOutput(0)->GetDataType();
  AssignOutput(ctx, mul->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
