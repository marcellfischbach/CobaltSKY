
#include <cobalt/graphics/shadergraph/cssgsplitfloat2.hh>

csSGSplitFloat2::csSGSplitFloat2()
  : csSGNode()
{
  SetName(CS_SPLIT_FLOAT2_NAME);
  AddInput(new csSGInput("v", false, true));
  AddOutput(new csSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new csSGOutput(eSGDT_Float, "y", "y"));
}

bool csSGSplitFloat2::Validate()
{
  bool success = csSGNode::Validate();

  csSGDataType dt = GetInputDataType(0);

  return success && dt == eSGDT_Float2;
}

