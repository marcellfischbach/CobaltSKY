#include <graphicsgl4/shadergraph/gl4sgadd.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void csSGAddGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  csSGNode *add = GetNode();
  ss << "(" << ctx.GetFullInputValue(add->GetInput(0)).c_str() << " + " <<
    ctx.GetFullInputValue(add->GetInput(1)).c_str() <<
    ")";

  csSGDataType dt = add->GetOutput(0)->GetDataType();// GetHigher(add->GetInput(0)->GetDataType(), add->GetInput(1)->GetDataType());
  AssignOutput(ctx, add->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
