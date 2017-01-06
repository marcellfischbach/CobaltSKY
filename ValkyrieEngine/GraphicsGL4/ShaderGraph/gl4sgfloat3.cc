#include <GraphicsGL4/ShaderGraph/gl4sgfloat3.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgfloateval.hh>
#include <sstream>



void vkSGFloat3GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();
  vkString src[] = {
    n->GetInput(0)->GetInput() ? ctx.GetInputValue(n->GetInput(0)) : "",
    n->GetInput(1)->GetInput() ? ctx.GetInputValue(n->GetInput(1)) : "",
    n->GetInput(2)->GetInput() ? ctx.GetInputValue(n->GetInput(2)) : ""
  };

  vkString attr[] = {
    n->GetInput(0)->GetInput() ? n->GetInput(0)->GetInput()->GetAttr() : "",
    n->GetInput(1)->GetInput() ? n->GetInput(1)->GetInput()->GetAttr() : "",
    n->GetInput(2)->GetInput() ? n->GetInput(2)->GetInput()->GetAttr() : ""
  };

  vkSGDataType dt[] = {
    n->GetInput(0)->GetInput() ? n->GetInput(0)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(1)->GetInput() ? n->GetInput(1)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(2)->GetInput() ? n->GetInput(2)->GetInput()->GetDataType() : eSGDT_Inval,
  };



  std::ostringstream ss;
  if (src[0] == src[1] && src[0] == src[2] && src[0].length() != 0 && attr[0].length() != 0 && attr[1].length() != 0 && attr[2].length() != 0)
  {
    ss << src[0] << "."
      << n->GetInput(0)->GetInput()->GetAttr()
      << n->GetInput(1)->GetInput()->GetAttr()
      << n->GetInput(2)->GetInput()->GetAttr();
  }
  else
  {
    ss << "vec3(" << eval(src, dt, n, ctx, 0, 3) << ")";
  }

  AssignOutput(ctx, n->GetOutput(0), ss.str(), "vec3");
}

