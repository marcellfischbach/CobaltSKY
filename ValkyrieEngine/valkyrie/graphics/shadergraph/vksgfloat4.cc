
#include <valkyrie/graphics/shadergraph/vksgfloat4.hh>

vkSGFloat4::vkSGFloat4()
  : vkSGNode()
{
  SetName(VK_FLOAT4_NAME);
  AddInput(new vkSGInput("x", true, true));
  AddInput(new vkSGInput("y", true, true));
  AddInput(new vkSGInput("z", true, true));
  AddInput(new vkSGInput("w", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float4, "v", ""));
}
