
#include <cobalt/graphics/shadergraph/cssgsplitfloat3.hh>

csSGSplitFloat3::csSGSplitFloat3()
  : csSGNode()
{
  SetName(CS_SPLIT_FLOAT3_NAME);
  AddInput(new csSGInput("v", false, true));
  AddOutput(new csSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new csSGOutput(eSGDT_Float, "y", "y"));
  AddOutput(new csSGOutput(eSGDT_Float, "z", "z"));
}

bool csSGSplitFloat3::Validate()
{
  bool success = csSGNode::Validate();

  csSGDataType dt = GetInputDataType(0);

  return success && dt == eSGDT_Float3;
}

