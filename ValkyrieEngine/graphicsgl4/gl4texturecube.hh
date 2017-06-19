#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4texture.hh>
#include <valkyrie/graphics/itexturecube.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4texturecube.refl.hh>


CS_CLASS();
class CSGRAPHICSGL4_API csTextureCubeGL4 : public CS_SUPER(csTextureGL4), public CS_SUPER(iTextureCube)
{
  CS_CLASS_GEN;
public:

  csTextureCubeGL4();
  virtual ~csTextureCubeGL4();

  bool Initialize(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth);


  // Implementation of the iTexture2DArray interface
  virtual csUInt16 GetWidth() const;
  virtual csUInt16 GetHeight() const;
  virtual csUInt16 GetDepth() const;

  virtual bool CopyData(csTextureCubeFace face, csUInt8 lod, csPixelFormat format, const void *data);

private:
  csPixelFormat m_format;

  csUInt16 m_width;
  csUInt16 m_height;
  csUInt16 m_depth;
};

