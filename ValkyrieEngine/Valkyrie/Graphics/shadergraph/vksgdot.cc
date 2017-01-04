
#include <Valkyrie/Graphics/shadergraph/vksgdot.hh>

vkSGDot::vkSGDot()
  : vkSGNode()
{
  SetName(VK_DOT_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float, "v"));
}


bool vkSGDot::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dtA = GetInputDataType(0);
  vkSGDataType dtB = GetInputDataType(1);
  if (!ScalarType(dtA, dtB))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  if (!SameType(dtA, dtB))
  {
    SetValidationMessage("Invalid mix of types");
    return false;
  }

  SetOutputDataType(0, eSGDT_Float);

  return success;
}
