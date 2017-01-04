
#include <Valkyrie/Graphics/shadergraph/vksgsub.hh>

vkSGSub::vkSGSub()
  : vkSGNode()
{
  SetName(VK_SUB_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddOutput(new vkSGOutput("v"));
}


bool vkSGSub::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dtA = GetInputDataType(0);
  vkSGDataType dtB = GetInputDataType(1);
  if (!ScalarType(dtA, dtB))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  if (!SameScalarType(dtA, dtB))
  {
    SetValidationMessage("Invalid mix of int/float");
    return false;
  }
  if (!SameTypeOrOne(dtA, dtB))
  {
    SetValidationMessage("Invalid combination of types in sub");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}
