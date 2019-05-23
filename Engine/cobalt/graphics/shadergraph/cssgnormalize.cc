
#include <cobalt/graphics/shadergraph/cssgnormalize.hh>




cs::SGNormalize::SGNormalize()
  : cs::SGNode()
{
  SetName(CS_NORMALIZE_NAME);
  AddInput(new cs::SGInput("a", true, true));
  AddOutput(new cs::SGOutput("v"));
}


bool cs::SGNormalize::Validate()
{
  bool success = cs::SGNode::Validate();

  cs::eSGDataType dtA = GetInputDataType(0);

  if (!FloatType(dtA))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  SetOutputDataType(0, dtA);
  return success;
}
