
#include <cobalt/graphics/shadergraph/cssglerp.hh>


csSGLerp::csSGLerp()
  : csSGNode()
{
  SetName(CS_LERP_NAME);
  AddInput(new csSGInput("a", true, true));
  AddInput(new csSGInput("b", true, true));
  AddInput(new csSGInput("x", true, true));
  AddOutput(new csSGOutput("v"));
}


bool csSGLerp::Validate()
{
  bool success = csSGNode::Validate();

  csSGDataType dtA = GetInputDataType(0);
  csSGDataType dtB = GetInputDataType(1);
  csSGDataType dtX = GetInputDataType(2);
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
