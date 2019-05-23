#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgtexture2d.refl.hh>

#define CS_TEXTURE2D_NAME "Texture/Texture2D"

namespace cs
{

CS_CLASS()
class CSE_API SGTexture2D : public CS_SUPER(cs::SGResourceNode)
{
  CS_CLASS_GEN;
public:
  SGTexture2D();
  virtual ~SGTexture2D() { }

};

}

