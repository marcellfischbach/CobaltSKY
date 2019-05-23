
#include <cobalt/graphics/shadergraph/cssgdefaulttexturecoordinate.hh>


cs::SGDefaultTextureCoordinate::SGDefaultTextureCoordinate()
  : cs::SGNode()
{
  SetName(CS_DEFAULT_TEXTURE_COORDINATE_NAME);
  AddOutput(new cs::SGOutput(cs::eSGDT_Float2, "uv"));
}