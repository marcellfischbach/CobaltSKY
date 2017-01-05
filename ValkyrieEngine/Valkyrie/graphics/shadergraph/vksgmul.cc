
#include <Valkyrie/graphics/shadergraph/vksgmul.hh>




vkSGMul::vkSGMul()
  : vkSGNode()
{
  SetName(VK_MUL_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddOutput(new vkSGOutput("v"));
}


bool vkSGMul::Validate()
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
    SetValidationMessage("Invalid combination of types in mul");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}
