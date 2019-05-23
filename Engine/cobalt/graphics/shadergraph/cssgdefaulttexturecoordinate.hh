#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgdefaulttexturecoordinate.refl.hh>

#define CS_DEFAULT_TEXTURE_COORDINATE_NAME "Texture/DefaultTextureCoordinate"

namespace cs
{

CS_CLASS()
class CSE_API SGDefaultTextureCoordinate : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGDefaultTextureCoordinate();
  virtual ~SGDefaultTextureCoordinate() { }

};

}
