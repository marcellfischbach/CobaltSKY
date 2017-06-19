#include <graphicsgl4/shadergraph/gl4sgdiv.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <sstream>

void csSGDivGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  csSGNode *div = GetNode();
  ss << "(" << ctx.GetFullInputValue(div->GetInput(0)).c_str() << " / " <<
    ctx.GetFullInputValue(div->GetInput(1)).c_str() <<
    ")";

  csSGDataType dt = div->GetOutput(0)->GetDataType();// GetHigher(div->GetInput(0)->GetDataType(), div->GetInput(1)->GetDataType());
  AssignOutput(ctx, div->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}
