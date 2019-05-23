
#include <cobalt/graphics/shadergraph/cssgadd.hh>

cs::SGAdd::SGAdd()
  : cs::SGNode()
{
  SetName(CS_ADD_NAME);
  AddInput(new cs::SGInput("a", true, true));
  AddInput(new cs::SGInput("b", true, true));
  AddOutput(new cs::SGOutput("v"));
}

bool cs::SGAdd::Validate()
{
  bool success = cs::SGNode::Validate();

  cs::eSGDataType dtA = GetInputDataType(0);
  cs::eSGDataType dtB = GetInputDataType(1);
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
    SetValidationMessage("Invalid combination of types in add");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}