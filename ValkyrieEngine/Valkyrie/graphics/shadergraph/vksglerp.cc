
#include <Valkyrie/graphics/shadergraph/vksglerp.hh>


vkSGLerp::vkSGLerp()
  : vkSGNode()
{
  SetName(VK_LERP_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddInput(new vkSGInput("x", true, true));
  AddOutput(new vkSGOutput("v"));
}


bool vkSGLerp::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dtA = GetInputDataType(0);
  vkSGDataType dtB = GetInputDataType(1);
  vkSGDataType dtX = GetInputDataType(2);
  if (!ScalarType(dtA, dtB) || !ScalarType(dtX))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  if (!SameScalarType(dtA, dtB) || !(SameScalarType(dtA, dtX) || dtX == eSGDT_Float))
  {
    SetValidationMessage("Invalid mix of int/float");
    return false;
  }
  if (!SameTypeOrOne(dtA, dtB) || !SameTypeOrOne(dtA, dtX))
  {
    SetValidationMessage("Invalid combination of types in mul");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}
