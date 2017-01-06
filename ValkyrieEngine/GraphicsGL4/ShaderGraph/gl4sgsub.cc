#include <GraphicsGL4/ShaderGraph/gl4sgsub.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <sstream>



void vkSGSubGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *sub = GetNode();
  ss << "(" << ctx.GetFullInputValue(sub->GetInput(0)).c_str() << " - " <<
    ctx.GetFullInputValue(sub->GetInput(1)).c_str() <<
    ")";

  vkSGDataType dt = sub->GetOutput(0)->GetDataType();// GetHigher(sub->GetInput(0)->GetDataType(), sub->GetInput(1)->GetDataType());
  AssignOutput(ctx, sub->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
