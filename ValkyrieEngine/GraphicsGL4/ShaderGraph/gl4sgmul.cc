#include <GraphicsGL4/ShaderGraph/gl4sgmul.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <sstream>



void vkSGMulGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *mul = GetNode();
  ss << "(" << ctx.GetFullInputValue(mul->GetInput(0)).c_str() << " * " <<
    ctx.GetFullInputValue(mul->GetInput(1)).c_str() <<
    ")";

  vkSGDataType dt = mul->GetOutput(0)->GetDataType();// GetHigher(mul->GetInput(0)->GetDataType(), mul->GetInput(1)->GetDataType());
  AssignOutput(ctx, mul->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
