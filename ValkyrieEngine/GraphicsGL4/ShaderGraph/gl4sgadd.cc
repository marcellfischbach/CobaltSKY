#include <GraphicsGL4/ShaderGraph/gl4sgadd.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <sstream>



void vkSGAddGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *add = GetNode();
  ss << "(" << ctx.GetFullInputValue(add->GetInput(0)).c_str() << " + " <<
    ctx.GetFullInputValue(add->GetInput(1)).c_str() <<
    ")";

  vkSGDataType dt = add->GetOutput(0)->GetDataType();// GetHigher(add->GetInput(0)->GetDataType(), add->GetInput(1)->GetDataType());
  AssignOutput(ctx, add->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
