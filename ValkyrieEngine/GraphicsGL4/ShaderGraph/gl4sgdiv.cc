#include <GraphicsGL4/ShaderGraph/gl4sgdiv.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <sstream>

void vkSGDivGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *div = GetNode();
  ss << "(" << ctx.GetFullInputValue(div->GetInput(0)).c_str() << " / " <<
    ctx.GetFullInputValue(div->GetInput(1)).c_str() <<
    ")";

  vkSGDataType dt = div->GetOutput(0)->GetDataType();// GetHigher(div->GetInput(0)->GetDataType(), div->GetInput(1)->GetDataType());
  AssignOutput(ctx, div->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
