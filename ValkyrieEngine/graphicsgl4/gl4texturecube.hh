#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4texture.hh>
#include <valkyrie/graphics/itexturecube.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4texturecube.refl.hh>


VK_CLASS();
class VKGRAPHICSGL4_API vkTextureCubeGL4 : public VK_SUPER(vkTextureGL4), public VK_SUPER(ITextureCube)
{
  VK_CLASS_GEN;
public:

  vkTextureCubeGL4();
  virtual ~vkTextureCubeGL4();

  bool Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth);


  // Implementation of the ITexture2DArray interface
  virtual vkUInt16 GetWidth() const;
  virtual vkUInt16 GetHeight() const;
  virtual vkUInt16 GetDepth() const;

  virtual bool CopyData(vkTextureCubeFace face, vkUInt8 lod, vkPixelFormat format, const void *data);

private:
  vkPixelFormat m_format;

  vkUInt16 m_width;
  vkUInt16 m_height;
  vkUInt16 m_depth;
};

