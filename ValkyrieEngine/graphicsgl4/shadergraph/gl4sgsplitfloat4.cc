#include <graphicsgl4/shadergraph/gl4sgsplitfloat4.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>



vkSGSplitFloat4GL4::vkSGSplitFloat4GL4()
  : vkSGNodeGL4()
{
  SetDoubleInlineEvaluateInput();
}

void vkSGSplitFloat4GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *node = GetNode();
  vkSGNodeGL4 *inputNode = ctx.GetNode(node->GetInput(0));
  if (!inputNode)
  {
    return;
  }

  vkString exp = ctx.GetInputValue(node->GetInput(0));
  ctx.SetOutputValue(node->GetOutput(0), exp);
  ctx.SetOutputValue(node->GetOutput(1), exp);
  ctx.SetOutputValue(node->GetOutput(2), exp);
  ctx.SetOutputValue(node->GetOutput(3), exp);

}