#include <graphicsgl4/shadergraph/gl4sgsplitfloat2.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>


vkSGSplitFloat2GL4::vkSGSplitFloat2GL4()
  : vkSGNodeGL4()
{
  SetDoubleInlineEvaluateInput();
}

void vkSGSplitFloat2GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
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

}