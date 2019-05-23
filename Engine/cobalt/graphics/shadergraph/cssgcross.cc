
#include <cobalt/graphics/shadergraph/cssgcross.hh>


cs::SGCross::SGCross()
  : cs::SGNode()
{
  SetName(CS_CROSS_NAME);
  AddInput(new cs::SGInput("a", true, true));
  AddInput(new cs::SGInput("b", true, true));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float3, "v"));
}


bool cs::SGCross::Validate()
{
  bool success = cs::SGNode::Validate();

  cs::eSGDataType dtA = GetInputDataType(0);
  cs::eSGDataType dtB = GetInputDataType(1);
  if (dtA != cs::eSGDT_Float3 || dtB != cs::eSGDT_Float3)
  {
    SetValidationMessage("Only Float3 supported for inputs");
    return false;
  }

  return success;
}

