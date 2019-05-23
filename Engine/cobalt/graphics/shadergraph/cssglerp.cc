
#include <cobalt/graphics/shadergraph/cssglerp.hh>


cs::SGLerp::SGLerp()
  : cs::SGNode()
{
  SetName(CS_LERP_NAME);
  AddInput(new cs::SGInput("a", true, true));
  AddInput(new cs::SGInput("b", true, true));
  AddInput(new cs::SGInput("x", true, true));
  AddOutput(new cs::SGOutput("v"));
}


bool cs::SGLerp::Validate()
{
  bool success = cs::SGNode::Validate();

  cs::eSGDataType dtA = GetInputDataType(0);
  cs::eSGDataType dtB = GetInputDataType(1);
  cs::eSGDataType dtX = GetInputDataType(2);
  if (!ScalarType(dtA, dtB) || !ScalarType(dtX))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  if (!SameScalarType(dtA, dtB) || !(SameScalarType(dtA, dtX) || dtX == cs::eSGDT_Float))
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
