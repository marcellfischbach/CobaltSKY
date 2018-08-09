#include <graphicsgl4/shadergraph/gl4sgoneminus.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void csSGOneMinusGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  csSGNode *mul = GetNode();
  ss << "(1 - " << ctx.GetFullInputValue(mul->GetInput(0)).c_str() << ")";

  csSGDataType dt = mul->GetOutput(0)->GetDataType();
  AssignOutput(ctx, mul->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
