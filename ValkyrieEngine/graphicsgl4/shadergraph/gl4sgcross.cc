#include <graphicsgl4/shadergraph/gl4sgcross.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void vkSGCrossGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *dot = GetNode();
  ss << "cross(" << ctx.GetFullInputValue(dot->GetInput(0)).c_str() << ", " <<
    ctx.GetFullInputValue(dot->GetInput(1)).c_str() <<
    ")";

  AssignOutput(ctx, dot->GetOutput(0), ss.str(), GetDataTypeVar(eSGDT_Float3));
}
