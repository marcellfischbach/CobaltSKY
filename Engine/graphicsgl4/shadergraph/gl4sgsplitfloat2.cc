#include <graphicsgl4/shadergraph/gl4sgsplitfloat2.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>


cs::SGSplitFloat2GL4::SGSplitFloat2GL4()
  : cs::SGNodeGL4()
{
  SetDoubleInlineEvaluateInput();
}

void cs::SGSplitFloat2GL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  cs::SGNode *node = GetNode();
  cs::SGNodeGL4 *inputNode = ctx.GetNode(node->GetInput(0));
  if (!inputNode)
  {
    return;
  }

  std::string exp = ctx.GetInputValue(node->GetInput(0));
  ctx.SetOutputValue(node->GetOutput(0), exp);
  ctx.SetOutputValue(node->GetOutput(1), exp);

}