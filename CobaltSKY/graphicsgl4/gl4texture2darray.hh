#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4texture.hh>
#include <cobalt/graphics/itexture2darray.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4texture2darray.refl.hh>

CS_CLASS();
class CSGRAPHICSGL4_API csTexture2DArrayGL4 : public CS_SUPER(csTextureGL4), public CS_SUPER(iTexture2DArray)
{
  CS_CLASS_GEN;
public:

  csTexture2DArrayGL4();
  virtual ~csTexture2DArrayGL4();

  bool Initialize(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps);


  // Implementation of the iTexture2DArray interface
  virtual csUInt16 GetWidth() const;
  virtual csUInt16 GetHeight() const;
  virtual csUInt16 GetLayers() const;

  virtual bool CopyData(csUInt8 lod, csPixelFormat format, const void *data);
  virtual bool CopyData(csUInt16 layer, csUInt8 lod, csPixelFormat format, const void *data);

private:
  csPixelFormat m_format;

  csUInt16 m_width;
  csUInt16 m_height;
  csUInt16 m_layers;
};
