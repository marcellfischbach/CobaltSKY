#include <graphicsgl4/shadergraph/gl4sgsplitfloat4.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>



csSGSplitFloat4GL4::csSGSplitFloat4GL4()
  : csSGNodeGL4()
{
  SetDoubleInlineEvaluateInput();
}

void csSGSplitFloat4GL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  csSGNode *node = GetNode();
  csSGNodeGL4 *inputNode = ctx.GetNode(node->GetInput(0));
  if (!inputNode)
  {
    return;
  }

  std::string exp = ctx.GetInputValue(node->GetInput(0));
  ctx.SetOutputValue(node->GetOutput(0), exp);
  ctx.SetOutputValue(node->GetOutput(1), exp);
  ctx.SetOutputValue(node->GetOutput(2), exp);
  ctx.SetOutputValue(node->GetOutput(3), exp);

}