
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/itexture2darray.hh>
#include <cobalt/graphics/itexturecube.hh>


#include <cobalt/graphics/cstexturewrapper.refl.hh>

CS_CLASS()
class CSE_API csTextureWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
public:
  csTextureWrapper(iTexture *texture = 0);

  CS_FORCEINLINE iTexture *Get()
  {
    return m_texture;
  }

  CS_FORCEINLINE const iTexture *Get() const
  {
    return m_texture;
  }

private:
  iTexture * m_texture;
};

CS_CLASS()
class CSE_API csTexture2DWrapper : public CS_SUPER(csTextureWrapper)
{
  CS_CLASS_GEN;
public:
  csTexture2DWrapper(iTexture2D *texture = 0);

  CS_FORCEINLINE iTexture2D* Get()
  {
    return m_texture2D;
  }

  CS_FORCEINLINE const iTexture2D* Get() const
  {
    return m_texture2D;
  }

private:
  iTexture2D * m_texture2D;

};


CS_CLASS()
class CSE_API csTexture2DArrayWrapper : public CS_SUPER(csTextureWrapper)
{
  CS_CLASS_GEN;
public:
  csTexture2DArrayWrapper(iTexture2DArray *texture = 0);

  CS_FORCEINLINE iTexture2DArray* Get()
  {
    return m_texture2DArray;
  }

  CS_FORCEINLINE const iTexture2DArray* Get() const
  {
    return m_texture2DArray;
  }

private:
  iTexture2DArray * m_texture2DArray;
};

CS_CLASS()
class CSE_API csTextureCubeWrapper : public CS_SUPER(csTextureWrapper)
{
  CS_CLASS_GEN;
public:
  csTextureCubeWrapper(iTextureCube *texture = 0);

  CS_FORCEINLINE iTextureCube* Get()
  {
    return m_textureCube;
  }

  CS_FORCEINLINE const iTextureCube* Get() const
  {
    return m_textureCube;
  }

private:
  iTextureCube * m_textureCube;
};
