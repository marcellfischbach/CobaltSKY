
#include <Valkyrie/graphics/shadergraph/vksgsplitfloat4.hh>

vkSGSplitFloat4::vkSGSplitFloat4()
  : vkSGNode()
{
  SetName(VK_SPLIT_FLOAT4_NAME);
  AddInput(new vkSGInput("v", false, true));
  AddOutput(new vkSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new vkSGOutput(eSGDT_Float, "y", "y"));
  AddOutput(new vkSGOutput(eSGDT_Float, "z", "z"));
  AddOutput(new vkSGOutput(eSGDT_Float, "w", "w"));
}

bool vkSGSplitFloat4::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dt = GetInputDataType(0);

  return success && dt == eSGDT_Float4;
}

