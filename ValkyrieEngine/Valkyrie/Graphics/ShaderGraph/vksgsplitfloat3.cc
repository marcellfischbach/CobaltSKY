
#include <Valkyrie/Graphics/ShaderGraph/vksgsplitfloat3.hh>

vkSGSplitFloat3::vkSGSplitFloat3()
  : vkSGNode()
{
  SetName(VK_SPLIT_FLOAT3_NAME);
  AddInput(new vkSGInput("v", false, true));
  AddOutput(new vkSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new vkSGOutput(eSGDT_Float, "y", "y"));
  AddOutput(new vkSGOutput(eSGDT_Float, "z", "z"));
}

bool vkSGSplitFloat3::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dt = GetInputDataType(0);

  return success && dt == eSGDT_Float3;
}

