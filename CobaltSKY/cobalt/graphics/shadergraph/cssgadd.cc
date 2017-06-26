
#include <cobalt/graphics/shadergraph/cssgadd.hh>

csSGAdd::csSGAdd()
  : csSGNode()
{
  SetName(CS_ADD_NAME);
  AddInput(new csSGInput("a", true, true));
  AddInput(new csSGInput("b", true, true));
  AddOutput(new csSGOutput("v"));
}

bool csSGAdd::Validate()
{
  bool success = csSGNode::Validate();

  csSGDataType dtA = GetInputDataType(0);
  csSGDataType dtB = GetInputDataType(1);
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