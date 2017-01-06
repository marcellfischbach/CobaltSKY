
#include <valkyrie/graphics/shadergraph/vksgconstfloat4.hh>


vkSGConstFloat4::vkSGConstFloat4()
  : vkSGNode()
{
  SetName(VK_CONST_FLOAT4_NAME);
  AddInput(new vkSGInput("x", true, false));
  AddInput(new vkSGInput("y", true, false));
  AddInput(new vkSGInput("z", true, false));
  AddInput(new vkSGInput("w", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float4, "v", ""));
}
