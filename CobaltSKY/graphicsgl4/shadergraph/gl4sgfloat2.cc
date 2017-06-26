#include <graphicsgl4/shadergraph/gl4sgfloat2.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <graphicsgl4/shadergraph/gl4sgfloateval.hh>
#include <sstream>

void csSGFloat2GL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  csSGNode *n = GetNode();
  csString src[] = {
    n->GetInput(0)->GetInput() ? ctx.GetInputValue(n->GetInput(0)) : "",
    n->GetInput(1)->GetInput() ? ctx.GetInputValue(n->GetInput(1)) : ""
  };

  csSGDataType dt[] = {
    n->GetInput(0)->GetInput() ? n->GetInput(0)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(1)->GetInput() ? n->GetInput(1)->GetInput()->GetDataType() : eSGDT_Inval,
  };
  std::ostringstream ss;
  if (src[0] == src[1] && src[0].length() != 0)
  {
    ss << src[0] << "."
      << n->GetInput(0)->GetInput()->GetAttr()
      << n->GetInput(1)->GetInput()->GetAttr();
  }
  else
  {
    ss << "vec2(" << eval(src, dt, n, ctx, 0, 2) << ")";
  }

  AssignOutput(ctx, n->GetOutput(0), ss.str(), "vec2");
}
