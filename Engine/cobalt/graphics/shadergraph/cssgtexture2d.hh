#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgtexture2d.refl.hh>

#define CS_TEXTURE2D_NAME "Texture/Texture2D"

CS_CLASS()
class CSE_API csSGTexture2D : public CS_SUPER(csSGResourceNode)
{
  CS_CLASS_GEN;
public:
  csSGTexture2D();
  virtual ~csSGTexture2D() { }

};
