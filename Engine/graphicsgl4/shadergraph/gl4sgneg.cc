#include <graphicsgl4/shadergraph/gl4sgneg.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void csSGNegGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  cs::SGNode *mul = GetNode();
  ss << "(-" << ctx.GetFullInputValue(mul->GetInput(0)).c_str() << ")";

  cs::eSGDataType dt = mul->GetOutput(0)->GetDataType();
  AssignOutput(ctx, mul->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
