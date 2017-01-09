#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4texture.hh>
#include <valkyrie/graphics/itexture2darray.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4texture2darray.refl.hh>

VK_CLASS();
class VKGRAPHICSGL4_API vkTexture2DArrayGL4 : public VK_SUPER(vkTextureGL4), public VK_SUPER(iTexture2DArray)
{
  VK_CLASS_GEN;
public:

  vkTexture2DArrayGL4();
  virtual ~vkTexture2DArrayGL4();

  bool Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers, bool mipmaps);


  // Implementation of the iTexture2DArray interface
  virtual vkUInt16 GetWidth() const;
  virtual vkUInt16 GetHeight() const;
  virtual vkUInt16 GetLayers() const;

  virtual bool CopyData(vkUInt8 lod, vkPixelFormat format, const void *data);
  virtual bool CopyData(vkUInt16 layer, vkUInt8 lod, vkPixelFormat format, const void *data);

private:
  vkPixelFormat m_format;

  vkUInt16 m_width;
  vkUInt16 m_height;
  vkUInt16 m_layers;
};
