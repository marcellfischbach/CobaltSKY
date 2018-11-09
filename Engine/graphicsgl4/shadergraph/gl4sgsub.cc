#include <graphicsgl4/shadergraph/gl4sgsub.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void csSGSubGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  csSGNode *sub = GetNode();
  ss << "(" << ctx.GetFullInputValue(sub->GetInput(0)).c_str() << " - " <<
    ctx.GetFullInputValue(sub->GetInput(1)).c_str() <<
    ")";

  csSGDataType dt = sub->GetOutput(0)->GetDataType();// GetHigher(sub->GetInput(0)->GetDataType(), sub->GetInput(1)->GetDataType());
  AssignOutput(ctx, sub->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
