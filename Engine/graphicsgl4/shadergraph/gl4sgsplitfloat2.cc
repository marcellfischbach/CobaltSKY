#include <graphicsgl4/shadergraph/gl4sgsplitfloat2.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>


csSGSplitFloat2GL4::csSGSplitFloat2GL4()
  : csSGNodeGL4()
{
  SetDoubleInlineEvaluateInput();
}

void csSGSplitFloat2GL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  cs::SGNode *node = GetNode();
  csSGNodeGL4 *inputNode = ctx.GetNode(node->GetInput(0));
  if (!inputNode)
  {
    return;
  }

  std::string exp = ctx.GetInputValue(node->GetInput(0));
  ctx.SetOutputValue(node->GetOutput(0), exp);
  ctx.SetOutputValue(node->GetOutput(1), exp);

}