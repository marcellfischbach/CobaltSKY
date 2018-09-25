
#include <cobalt/graphics/shadergraph/cssgdefaulttexturecoordinate.hh>


csSGDefaultTextureCoordinate::csSGDefaultTextureCoordinate()
  : csSGNode()
{
  SetName(CS_DEFAULT_TEXTURE_COORDINATE_NAME);
  AddOutput(new csSGOutput(eSGDT_Float2, "uv"));
}