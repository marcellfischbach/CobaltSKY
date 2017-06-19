#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4texture.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4texture2d.refl.hh>

CS_CLASS()
class CSGRAPHICSGL4_API csTexture2DGL4 : public CS_SUPER(csTextureGL4), public CS_SUPER(iTexture2D)
{
  CS_CLASS_GEN;
public:

  csTexture2DGL4();
  virtual ~csTexture2DGL4();

  bool Initialize(csPixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps);


  // Implementation of the iTexture2D interface
  virtual csUInt16 GetWidth() const;
  virtual csUInt16 GetHeight() const;
  virtual csUInt16 GetNumberOfLODs() const;

  virtual bool CopyData(csUInt8 layer, csPixelFormat format, const void *data);

private:
  csPixelFormat m_format;

  csUInt16 m_width;
  csUInt16 m_height;
  csUInt16 m_lods;
};
