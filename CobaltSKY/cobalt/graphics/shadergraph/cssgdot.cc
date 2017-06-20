
#include <cobalt/graphics/shadergraph/cssgdot.hh>

csSGDot::csSGDot()
  : csSGNode()
{
  SetName(CS_DOT_NAME);
  AddInput(new csSGInput("a", true, true));
  AddInput(new csSGInput("b", true, true));
  AddOutput(new csSGOutput(eSGDT_Float, "v"));
}


bool csSGDot::Validate()
{
  bool success = csSGNode::Validate();

  csSGDataType dtA = GetInputDataType(0);
  csSGDataType dtB = GetInputDataType(1);
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
