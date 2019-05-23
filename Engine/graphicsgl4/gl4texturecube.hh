#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4texture.hh>
#include <cobalt/graphics/itexturecube.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4texturecube.refl.hh>


namespace cs
{

CS_CLASS();
class CSGRAPHICSGL4_API TextureCubeGL4 : public CS_SUPER(cs::TextureGL4), public CS_SUPER(cs::iTextureCube)
{
  CS_CLASS_GEN;
public:

  TextureCubeGL4();
  virtual ~TextureCubeGL4();

  bool Initialize(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth);


  // Implementation of the cs::iTexture2DArray interface
  virtual csUInt16 GetWidth() const;
  virtual csUInt16 GetHeight() const;
  virtual csUInt16 GetDepth() const;

  virtual bool CopyData(cs::eTextureCubeFace face, csUInt8 lod, cs::ePixelFormat format, const void* data);

private:
  cs::ePixelFormat m_format;

  csUInt16 m_width;
  csUInt16 m_height;
  csUInt16 m_depth;
};


}

