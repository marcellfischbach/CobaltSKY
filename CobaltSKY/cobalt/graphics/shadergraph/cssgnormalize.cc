
#include <cobalt/graphics/shadergraph/cssgnormalize.hh>




csSGNormalize::csSGNormalize()
  : csSGNode()
{
  SetName(CS_NORMALIZE_NAME);
  AddInput(new csSGInput("a", true, true));
  AddOutput(new csSGOutput("v"));
}


bool csSGNormalize::Validate()
{
  bool success = csSGNode::Validate();

  csSGDataType dtA = GetInputDataType(0);

  if (!FloatType(dtA))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  SetOutputDataType(0, dtA);
  return success;
}
