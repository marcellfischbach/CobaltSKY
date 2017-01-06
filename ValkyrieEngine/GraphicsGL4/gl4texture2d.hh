#pragma once


#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/gl4texture.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <GL/glew.h>
#include <GraphicsGL4/gl4texture2d.refl.hh>

VK_CLASS()
class VKGRAPHICSGL4_API vkTexture2DGL4 : public VK_SUPER(vkTextureGL4), public VK_SUPER(ITexture2D)
{
  VK_CLASS_GEN;
public:

  vkTexture2DGL4();
  virtual ~vkTexture2DGL4();

  bool Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height, bool mipmaps);


  // Implementation of the ITexture2D interface
  virtual vkUInt16 GetWidth() const;
  virtual vkUInt16 GetHeight() const;

  virtual bool CopyData(vkUInt8 layer, vkPixelFormat format, const void *data);

private:
  vkPixelFormat m_format;

  vkUInt16 m_width;
  vkUInt16 m_height;

};
