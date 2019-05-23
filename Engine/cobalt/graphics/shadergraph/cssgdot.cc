
#include <cobalt/graphics/shadergraph/cssgdot.hh>

cs::SGDot::SGDot()
  : cs::SGNode()
{
  SetName(CS_DOT_NAME);
  AddInput(new cs::SGInput("a", true, true));
  AddInput(new cs::SGInput("b", true, true));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "v"));
}


bool cs::SGDot::Validate()
{
  bool success = cs::SGNode::Validate();

  cs::eSGDataType dtA = GetInputDataType(0);
  cs::eSGDataType dtB = GetInputDataType(1);
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

  SetOutputDataType(0, cs::eSGDT_Float);

  return success;
}
