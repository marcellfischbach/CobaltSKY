#include <graphicsgl4/shadergraph/gl4sglerp.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>


void cs::SGLerpGL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  cs::SGNode *lerp = GetNode();
  ss << "mix(" << ctx.GetFullInputValue(lerp->GetInput(0)).c_str() << ", " <<
    ctx.GetFullInputValue(lerp->GetInput(1)).c_str() << ", " <<
    ctx.GetFullInputValue(lerp->GetInput(2)).c_str() <<
    ")";
  cs::eSGDataType dt = lerp->GetOutput(0)->GetDataType();
  AssignOutput(ctx, lerp->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
