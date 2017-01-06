#include <GraphicsGL4/ShaderGraph/gl4sgdot.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <sstream>


void vkSGDotGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *dot = GetNode();
  ss << "dot(" << ctx.GetFullInputValue(dot->GetInput(0)).c_str() << ", " <<
    ctx.GetFullInputValue(dot->GetInput(1)).c_str() <<
    ")";

  AssignOutput(ctx, dot->GetOutput(0), ss.str(), GetDataTypeVar(eSGDT_Float));
}
