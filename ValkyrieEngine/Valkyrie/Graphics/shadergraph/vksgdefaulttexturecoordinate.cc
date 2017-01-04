
#include <Valkyrie/Graphics/shadergraph/vksgdefaulttexturecoordinate.hh>


vkSGDefaultTextureCoordinate::vkSGDefaultTextureCoordinate()
  : vkSGNode()
{
  SetName(VK_DEFAULT_TEXTURE_COORDINATE_NAME);
  AddOutput(new vkSGOutput(eSGDT_Float2, "uv"));
}