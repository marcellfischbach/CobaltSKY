#include <GraphicsGL4/ShaderGraph/gl4sgfloat2.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgfloateval.hh>
#include <sstream>

void vkSGFloat2GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();
  vkString src[] = {
    n->GetInput(0)->GetInput() ? ctx.GetInputValue(n->GetInput(0)) : "",
    n->GetInput(1)->GetInput() ? ctx.GetInputValue(n->GetInput(1)) : ""
  };

  vkSGDataType dt[] = {
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
