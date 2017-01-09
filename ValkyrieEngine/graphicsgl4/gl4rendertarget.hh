#pragma once

#include <graphicsgl4/gl4export.hh>
#include <valkyrie/graphics/irendertarget.hh>
#include <vector>
#include <GL/glew.h>
#include <graphicsgl4/gl4rendertarget.refl.hh>

class vkTextureGL4;

VK_CLASS()
class VKGRAPHICSGL4_API vkRenderTargetGL4 : public VK_SUPER(iRenderTarget)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkRenderTargetGL4();
  vkRenderTargetGL4(GLuint name, vkUInt16 width, vkUInt16 height);
  virtual ~vkRenderTargetGL4();

  void Setup(GLuint name, vkUInt16 width, vkUInt16 heigth);
  void Bind();


  vkUInt16 GetWidth() const;
  vkUInt16 GetHeight() const;


  void AddColorTexture(iTexture *color);
  void SetDepthTexture(iTexture *depth);
  void SetDepthBuffer(vkUInt16 width, vkUInt16 height);
  void Initialize(vkUInt16 width, vkUInt16 height);
  bool Finilize();


  virtual iTexture *GetColorBuffer(vkUInt8 buffer) const;
  virtual iTexture *GetDepthBuffer() const;

private:
  vkUInt16 m_width;
  vkUInt16 m_height;

  std::vector<vkTextureGL4*> m_colorTextures;
  vkTextureGL4 *m_depthTexture;
  GLuint m_depthBuffer;
  GLuint m_name;
  bool m_provided;
};


