#include <graphicsgl4/shadergraph/gl4sgdot.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>


void csSGDotGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  cs::SGNode *dot = GetNode();
  ss << "dot(" << ctx.GetFullInputValue(dot->GetInput(0)).c_str() << ", " <<
    ctx.GetFullInputValue(dot->GetInput(1)).c_str() <<
    ")";

  AssignOutput(ctx, dot->GetOutput(0), ss.str(), GetDataTypeVar(cs::eSGDT_Float));
}
