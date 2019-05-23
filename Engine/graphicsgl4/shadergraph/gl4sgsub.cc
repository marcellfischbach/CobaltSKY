#include <graphicsgl4/shadergraph/gl4sgsub.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>



void cs::SGSubGL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  cs::SGNode *sub = GetNode();
  ss << "(" << ctx.GetFullInputValue(sub->GetInput(0)).c_str() << " - " <<
    ctx.GetFullInputValue(sub->GetInput(1)).c_str() <<
    ")";

  cs::eSGDataType dt = sub->GetOutput(0)->GetDataType();// GetHigher(sub->GetInput(0)->GetDataType(), sub->GetInput(1)->GetDataType());
  AssignOutput(ctx, sub->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
