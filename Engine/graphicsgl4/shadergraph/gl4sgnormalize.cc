#include <graphicsgl4/shadergraph/gl4sgnormalize.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void cs::SGNormalizeGL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  cs::SGNode *normalize = GetNode();
  ss << "normalize(" << ctx.GetFullInputValue(normalize->GetInput(0)).c_str() << ")";

  cs::eSGDataType dt = normalize->GetOutput(0)->GetDataType();// GetHigher(mul->GetInput(0)->GetDataType(), mul->GetInput(1)->GetDataType());
  AssignOutput(ctx, normalize->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
