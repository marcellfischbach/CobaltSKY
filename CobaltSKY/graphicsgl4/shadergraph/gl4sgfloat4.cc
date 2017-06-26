#include <graphicsgl4/shadergraph/gl4sgfloat4.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <graphicsgl4/shadergraph/gl4sgfloateval.hh>
#include <sstream>

void csSGFloat4GL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  csSGNode *n = GetNode();
  csString src[] = {
    n->GetInput(0)->GetInput() ? ctx.GetInputValue(n->GetInput(0)) : "",
    n->GetInput(1)->GetInput() ? ctx.GetInputValue(n->GetInput(1)) : "",
    n->GetInput(2)->GetInput() ? ctx.GetInputValue(n->GetInput(2)) : "",
    n->GetInput(3)->GetInput() ? ctx.GetInputValue(n->GetInput(3)) : ""
  };

  csSGDataType dt[] = {
    n->GetInput(0)->GetInput() ? n->GetInput(0)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(1)->GetInput() ? n->GetInput(1)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(2)->GetInput() ? n->GetInput(2)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(3)->GetInput() ? n->GetInput(3)->GetInput()->GetDataType() : eSGDT_Inval,
  };
  std::ostringstream ss;
  if (src[0] == src[1] && src[0] == src[2] && src[0] == src[3] && src[0].length() != 0)
  {
    ss << src[0] << "."
      << n->GetInput(0)->GetInput()->GetAttr()
      << n->GetInput(1)->GetInput()->GetAttr()
      << n->GetInput(2)->GetInput()->GetAttr();
  }
  else
  {
    ss << "vec4(" << eval(src, dt, n, ctx, 0, 4) << ")";
  }


  AssignOutput(ctx, n->GetOutput(0), ss.str(), "vec4");
}