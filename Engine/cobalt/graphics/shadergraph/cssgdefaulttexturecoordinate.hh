#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgdefaulttexturecoordinate.refl.hh>

#define CS_DEFAULT_TEXTURE_COORDINATE_NAME "Texture/DefaultTextureCoordinate"

CS_CLASS()
class CSE_API csSGDefaultTextureCoordinate : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGDefaultTextureCoordinate();
  virtual ~csSGDefaultTextureCoordinate() { }

};
