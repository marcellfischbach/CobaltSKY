
#include <valkyrie/graphics/shadergraph/vksgsplitfloat2.hh>

vkSGSplitFloat2::vkSGSplitFloat2()
  : vkSGNode()
{
  SetName(VK_SPLIT_FLOAT2_NAME);
  AddInput(new vkSGInput("v", false, true));
  AddOutput(new vkSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new vkSGOutput(eSGDT_Float, "y", "y"));
}

bool vkSGSplitFloat2::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dt = GetInputDataType(0);

  return success && dt == eSGDT_Float2;
}

