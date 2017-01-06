#include <GraphicsGL4/ShaderGraph/gl4sglerp.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <sstream>


void vkSGLerpGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *lerp = GetNode();
  ss << "mix(" << ctx.GetFullInputValue(lerp->GetInput(0)).c_str() << ", " <<
    ctx.GetFullInputValue(lerp->GetInput(1)).c_str() << ", " <<
    ctx.GetFullInputValue(lerp->GetInput(2)).c_str() <<
    ")";
  vkSGDataType dt = lerp->GetOutput(0)->GetDataType();
  AssignOutput(ctx, lerp->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
