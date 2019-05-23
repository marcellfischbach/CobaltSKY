#include <graphicsgl4/shadergraph/gl4sgmul.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void cs::SGMulGL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  cs::SGNode *mul = GetNode();
  ss << "(" << ctx.GetFullInputValue(mul->GetInput(0)).c_str() << " * " <<
    ctx.GetFullInputValue(mul->GetInput(1)).c_str() <<
    ")";

  cs::eSGDataType dt = mul->GetOutput(0)->GetDataType();// GetHigher(mul->GetInput(0)->GetDataType(), mul->GetInput(1)->GetDataType());
  AssignOutput(ctx, mul->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
