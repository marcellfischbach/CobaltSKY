
#include <Valkyrie/Graphics/shadergraph/vksgfloat3.hh>

vkSGFloat3::vkSGFloat3()
  : vkSGNode()
{
  SetName(VK_FLOAT3_NAME);
  AddInput(new vkSGInput("x", true, true));
  AddInput(new vkSGInput("y", true, true));
  AddInput(new vkSGInput("z", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float3, "v", ""));
}
