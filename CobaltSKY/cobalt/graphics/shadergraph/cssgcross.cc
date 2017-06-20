
#include <cobalt/graphics/shadergraph/cssgcross.hh>


csSGCross::csSGCross()
  : csSGNode()
{
  SetName(CS_CROSS_NAME);
  AddInput(new csSGInput("a", true, true));
  AddInput(new csSGInput("b", true, true));
  AddOutput(new csSGOutput(eSGDT_Float3, "v"));
}


bool csSGCross::Validate()
{
  bool success = csSGNode::Validate();

  csSGDataType dtA = GetInputDataType(0);
  csSGDataType dtB = GetInputDataType(1);
  if (dtA != eSGDT_Float3 || dtB != eSGDT_Float3)
  {
    SetValidationMessage("Only Float3 supported for inputs");
    return false;
  }

  return success;
}

