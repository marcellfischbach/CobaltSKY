#include <Valkyrie/Graphics/ShaderGraph/vksgvarfloat3.hh>

vkSGVarFloat3::vkSGVarFloat3()
  : vkSGResourceNode()
{
  SetName(VK_VAR_FLOAT3_NAME);
  SetResourceType(eSPT_Vector3);
  AddOutput(new vkSGOutput(eSGDT_Float3, "v"));
}
