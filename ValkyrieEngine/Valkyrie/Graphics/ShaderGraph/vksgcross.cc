
#include <Valkyrie/Graphics/ShaderGraph/vksgcross.hh>


vkSGCross::vkSGCross()
  : vkSGNode()
{
  SetName(VK_CROSS_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float3, "v"));
}


bool vkSGCross::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dtA = GetInputDataType(0);
  vkSGDataType dtB = GetInputDataType(1);
  if (dtA != eSGDT_Float3 || dtB != eSGDT_Float3)
  {
    SetValidationMessage("Only Float3 supported for inputs");
    return false;
  }

  return success;
}

