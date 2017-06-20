
#include <cobalt/graphics/shadergraph/cssgsplitfloat4.hh>

csSGSplitFloat4::csSGSplitFloat4()
  : csSGNode()
{
  SetName(CS_SPLIT_FLOAT4_NAME);
  AddInput(new csSGInput("v", false, true));
  AddOutput(new csSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new csSGOutput(eSGDT_Float, "y", "y"));
  AddOutput(new csSGOutput(eSGDT_Float, "z", "z"));
  AddOutput(new csSGOutput(eSGDT_Float, "w", "w"));
}

bool csSGSplitFloat4::Validate()
{
  bool success = csSGNode::Validate();

  csSGDataType dt = GetInputDataType(0);

  return success && dt == eSGDT_Float4;
}

