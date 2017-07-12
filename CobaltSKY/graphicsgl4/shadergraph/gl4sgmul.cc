#include <graphicsgl4/shadergraph/gl4sgmul.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void csSGMulGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  csSGNode *mul = GetNode();
  ss << "(" << ctx.GetFullInputValue(mul->GetInput(0)).c_str() << " * " <<
    ctx.GetFullInputValue(mul->GetInput(1)).c_str() <<
    ")";

  csSGDataType dt = mul->GetOutput(0)->GetDataType();// GetHigher(mul->GetInput(0)->GetDataType(), mul->GetInput(1)->GetDataType());
  AssignOutput(ctx, mul->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}