#include <Valkyrie/Graphics/ShaderGraph/vksgvarfloat2.hh>

vkSGVarFloat2::vkSGVarFloat2()
  : vkSGResourceNode()
{
  SetName(VK_VAR_FLOAT2_NAME);
  SetResourceType(eSPT_Vector2);
  AddOutput(new vkSGOutput(eSGDT_Float2, "v"));
}
