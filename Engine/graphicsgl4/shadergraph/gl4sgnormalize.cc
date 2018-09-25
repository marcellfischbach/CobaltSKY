#include <graphicsgl4/shadergraph/gl4sgnormalize.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void csSGNormalizeGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  csSGNode *normalize = GetNode();
  ss << "normalize(" << ctx.GetFullInputValue(normalize->GetInput(0)).c_str() << ")";

  csSGDataType dt = normalize->GetOutput(0)->GetDataType();// GetHigher(mul->GetInput(0)->GetDataType(), mul->GetInput(1)->GetDataType());
  AssignOutput(ctx, normalize->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
