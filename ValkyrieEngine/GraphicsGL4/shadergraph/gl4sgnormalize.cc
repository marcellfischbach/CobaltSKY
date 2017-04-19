#include <graphicsgl4/shadergraph/gl4sgnormalize.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void vkSGNormalizeGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *normalize = GetNode();
  ss << "normalize(" << ctx.GetFullInputValue(normalize->GetInput(0)).c_str() << ")";

  vkSGDataType dt = normalize->GetOutput(0)->GetDataType();// GetHigher(mul->GetInput(0)->GetDataType(), mul->GetInput(1)->GetDataType());
  AssignOutput(ctx, normalize->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
