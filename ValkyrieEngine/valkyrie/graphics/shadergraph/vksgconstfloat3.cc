
#include <valkyrie/graphics/shadergraph/vksgconstfloat3.hh>


vkSGConstFloat3::vkSGConstFloat3()
  : vkSGNode()
{
  SetName(VK_CONST_FLOAT3_NAME);
  AddInput(new vkSGInput("x", true, false));
  AddInput(new vkSGInput("y", true, false));
  AddInput(new vkSGInput("z", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float3, "v", ""));
}
